#pragma once
#include "core/line.h"
#include "core/strbuf.h"
#include "core/doccmd.h"

struct cursor {
	uint flags;
	uint row;		/**< Line number on visible screen buffer */
	uint col;
};

typedef struct document {
	uint flags;
	strbuf path;	/**< Absolute path of the document. Empty for a new doc */
	uint numlines;
	line* lines;	/**< document consists of lines */
	struct cursor cursor;
} document;


// Create a new empty document
document doc_create();
document doc_createfrom_file(strbuf path);

// Save a document to disk at doc->path
int doc_save_to_disk(document* doc);

// Input processing
int doc_exec(document* doc, doccmd dc);

void doc_pprint(document* doc);
void doc_pprint_lines(document* doc);

