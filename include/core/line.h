#pragma once
#include "mytypes.h"
#include "core/strbuf.h"

typedef struct line {
	uint flags;
	uint num; // Line number
	uint cur; // Cursor
	strbuf sb;
} line;


line ln_create();
void ln_pprint(line* ln);

