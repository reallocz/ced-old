#include "core/document.h"
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"

// Default number of lines
#define DOCDEF_NUMLINES 2

/** Document was loaded from disk */
static const uint f_fromdisk = 1 << 5;


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


document doc_create()
{
	document doc;
	doc.flags = 0;
	doc.path = sb_create(32);
	doc.numlines = DOCDEF_NUMLINES;
	doc.lines = _doc_createlines(DOCDEF_NUMLINES);
	flag_set(&doc.flags, f_init);
	return doc;
}


document doc_createfrom_file(strbuf path)
{
	document doc;
	doc.flags = 0;
	doc.path = path;
	char* pathc = sb_create_cstr(&doc.path);
	strbuf filecontents = sb_createfrom_file(pathc);
	sb_destroy_cstr(pathc);
	printf("'%s'\n", sb_create_cstr(&doc.path));

	// Split filecontents into lines
	uint count;
	strbuf* splits;
	sb_splitlines(&filecontents, &splits, &count);
	if(count > 1) count--; // Ignore the split after EOF
	printf("count; %d\n", count);
	doc.numlines = count;
	doc.lines = _doc_createlines(count);

	for(uint i = 0; i < count; ++i) {
		doc.lines[i].sb = splits[i];
	}

	flag_set(&doc.flags, f_init | f_fromdisk);
	return doc;
}


int doc_exec(document* doc, doccmd cmd)
{
}

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

