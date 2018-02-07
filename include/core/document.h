#pragma once
#include "flags.h"
#include "core/line.h"

typedef struct document {
	uint flags;
	char* path;
	line* lines;	
} document;
