#include "document/document.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "flags.h"
#include "mymaths.h"
#include "ds/strbuf.h"

// Default number of lines
#define DOCDEF_NUMLINES 2

/** Get the line under the cursor */
#define DOC_CURLINE(doc) \
	(&doc->lines[doc->cursor.row])

/** Document was loaded from disk */
static const uint f_fromdisk = 1 << 5;

/** Doccmd exec functions */
static int _exec_insert(document* doc, doccmd cmd);
static int _exec_move(document* doc, doccmd cmd);
static int _exec_edit(document* doc, doccmd cmd);
static int _exec_save(document* doc, doccmd cmd);

/** Allocate lines and assign a number to them. */
static line* _doc_createlines(uint numlines)
{
	line* lines = malloc(numlines * sizeof(line));
	if(lines == NULL) {
		printf("E: %s(numlines = %d): out of memory.\n", __func__, numlines);
		exit(1);
	}
	// Init lines and assign a number
	for(uint i = 0; i < numlines; ++i) {
		lines[i] = ln_create();
		lines[i].num = i;
	}
	return lines;
}


/** Returns document with everything set to 0, with no allocations.
 * - document.path is not set!
 */
document _doc_create_empty() {
	document doc;
	doc.flags = 0;
	doc.numlines = 0;
	doc.lines = NULL;
	doc.cursor = (struct cursor) {.flags = 0, .row = 0, .col = 0 };
	return doc;
}

document doc_create()
{
	document doc = _doc_create_empty();
	doc.path = sb_create(32);
	doc.numlines = DOCDEF_NUMLINES;
	doc.lines = _doc_createlines(DOCDEF_NUMLINES);
	flag_set(&doc.flags, f_init);
	return doc;
}


document doc_createfrom_file(strbuf path)
{
	document doc = _doc_create_empty();
	doc.path = path;

	char* pathc = sb_create_cstr(&doc.path);
	strbuf filecontents = sb_createfrom_file(pathc);
	sb_destroy_cstr(pathc);

	// Split filecontents into lines
	uint count;
	strbuf* splits = NULL;
	sb_splitlines(&filecontents, &splits, &count);
	sb_destroy(&filecontents);	/* Safe to destroy after split */

	// Ignore the split after EOF
	if(count > 1) {
		sb_destroy(&splits[count]);
		count--;
	}

	printf("%s: num lines = %d\n", __func__, count);
	doc.numlines = count;
	doc.lines = _doc_createlines(count);

	// copy splits to lines
	for(uint i = 0; i < count; ++i) {
		doc.lines[i].sb = splits[i];
	}

	flag_set(&doc.flags, f_init | f_fromdisk);
	return doc;
}

// utils

// Cursor functions
static int _cur_move_right(document* doc, uint cols)
{
	line* ln = DOC_CURLINE(doc);
	struct cursor* cur = &doc->cursor;
	cur->col = MIN(cur->col+cols, ln->sb.len);
	return 0;
}

static int _cur_move_left(document* doc, uint cols)
{
	line* ln = DOC_CURLINE(doc);
	struct cursor* cur = &doc->cursor;
	cur->col = MAX(cur->col+cols, ln->sb.len);
	return 0;
}


// Exec functions
static int _exec_insert(document* doc, doccmd cmd)
{
	assert(doc);
	line* ln = DOC_CURLINE(doc);
	sb_append_char(&ln->sb, cmd.data.c);
	_cur_move_right(doc, 1);
	return 0;
}


int doc_exec(document* doc, doccmd cmd)
{
	switch (cmd.type) {
		case INSERT:
			_exec_insert(doc, cmd);
			break;
		default:
			printf("NOP\n");
			break;
	}
	return 0;
}



// DEBUGGING AND PPRINTING
void doc_pprint(document* doc)
{
	printf("[[document]] ");
	printf("path=\"");
		sb_print_string(&doc->path);
	printf("\" ");
	printf("numlines=%d ", doc->numlines);
	printf("[[/document]]\n");
}

void doc_pprint_lines(document* doc)
{
	for(uint i = 0; i < doc->numlines; ++i ) {
		sb_pprint(&doc->lines[i].sb);
	}
}

