#pragma once
#include "mytypes.h"

typedef struct shader {
	uint flags;
	int id;
} shader;


shader shader_create(const char* vpath, const char* fpath);

void shader_use(shader* sh);

