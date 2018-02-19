#pragma once
#include "mytypes.h"
#include "mymaths.h"

typedef struct font_t font;

typedef struct glyph {
	uint index;			/**< Glyph index */
	char ch;			/**< Character of the glyph */
	vec2 advance;		/**< int x, y */
	int bitmap_left;
	int bitmap_top;
	int bitmap_width;
	int bitmap_height;
} glyph;

/** initialize font submodule for use */
int font_initmodule();

/** Load up the font */
font* font_load(const char* path);

/** Deallocate font and glyph resources */
void font_destroy(font* f);

/// Properties
int font_set_size(font* font, uint size);

// Glyph
const glyph* font_glyph_get(const font* fnt, char ch);

// pprint and debug
void font_pprint(const font* fnt);
void font_glyph_pprint(const glyph* g);
/** Print glyph to the console */
void font_printarb(unsigned char* buffer, uint width, uint rows);

