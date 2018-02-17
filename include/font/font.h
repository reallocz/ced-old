#pragma once
#include "mytypes.h"

typedef struct font_t font;

/** initialize font submodule for use */
int font_initmodule();

/** Load up the font */
font* font_load(const char* path);
void font_destroy(font* f);

int font_set_size(font* font, uint size);

