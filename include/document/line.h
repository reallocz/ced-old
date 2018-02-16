#pragma once
#include "mytypes.h"
#include "ds/strbuf.h"

typedef struct line {
	uint flags;
	uint num; // Line number
	uint cur; // Cursor
	strbuf sb;
} line;


line ln_create();

void ln_destroy();

void ln_pprint(line* ln);

