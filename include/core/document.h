#pragma once
#include "core/line.h"
#include "core/strbuf.h"
#include "core/doccmd.h"

typedef struct document {
	uint flags;
	strbuf path;	/**< Absolute path of the document. Empty for a new doc */
	line* lines;	/**< document consists of lines */
} document;


// Create a new empty document
document doc_create();
document doc_createfrom_file(strbuf path);

// Save a document to disk at doc->path
int doc_save_to_disk(document* doc);


// Input processing
int doc_exec(doccmd dc);

