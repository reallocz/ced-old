#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <assert.h>

#include "font/font.h"
#include "mytypes.h"
#include "flags.h"

/** Default font size */
#define FNTDEF_SIZE 24
/** Default PPI(DPI?) for 1366/768 15.5' (https://www.sven.de/dpi/) */
#define FNTDEF_RES 100
/** We load only the standard ascii glyph's right now */
#define FNTDEF_ASCIIBEGIN 32
#define FNTDEF_ASCIIEND	 127

struct font_t {
	uint flags;
	FT_Face ft_face;
	uint size;
	uint resolution;

	glyph* glyphs;		/**< Array of glyphs */
	uint glyphcount;	/**< Count of initialized glyphs */
	uint glyphcapacity; /**< Max glyphcount */
};

/* Init only once */
static FT_Library FTLIB = NULL;

/** Initialize freetype2.
 * Freetype2 must be initialized before using any of the font_ functions.
 */
static int _font_initfreetype()
{
	if(FTLIB) {
		printf("E: %s: double init!\n", __func__);
		return 1;
	}
	int error = 0;
	error = FT_Init_FreeType(&FTLIB);
	if(error) {
		printf("E: %s: failed to init freetype!\n", __func__);
		return 1;
	}
	assert(FTLIB);
	return 0;
}

int font_initmodule()
{
	return _font_initfreetype();
}

/** Exit if freetype's not init */
static void _check_init()
{
	if(! FTLIB) {
		printf("E: %s: freetype is not initialized!\n", __func__);
		exit(1);
	}
}


/** Set everything to 0/defaults */
static void _init_font(font* f)
{
	assert(f);
	f->flags = 0;
	f->ft_face = NULL;
	f->size = 0;
	f->resolution = 0;
	// Glyphs
	f->glyphs = NULL;
	f->glyphcapacity = 0;
	f->glyphcount = 0;
}

static void _font_glyph_copy(glyph* dst, const glyph* src)
{
	dst->index = src->index;
	dst->ch = src->ch;
	dst->advance = src->advance;
	dst->bitmap_left = src->bitmap_left;
	dst->bitmap_top = src->bitmap_top;
	dst->bitmap_width = src->bitmap_width;
	dst->bitmap_height = src->bitmap_height;
}

static int _font_glyph_reserve(font* fnt, uint num)
{
	assert(fnt);
	fnt->glyphs = malloc(num * sizeof(glyph));
	if(!fnt->glyphs) {
		printf("E: %s(num=%d): failed to allocate memory.\n", __func__, num);
		return 1;
	}
	fnt->glyphcapacity = num;
	fnt->glyphcount = 0;
	return 0;
}

static int _font_glyph_add(font* fnt, const glyph* g)
{
	assert(fnt && g);
	// TODO Check for duplicates on key glyph.index
	if(fnt->glyphcount < fnt->glyphcapacity) {
		glyph* dst = &fnt->glyphs[fnt->glyphcount++];
		_font_glyph_copy(dst, g);
		return 0;
	} else {
		printf("E: %s: out of glyphs storage: %d / %d\n", __func__,
				fnt->glyphcount, fnt->glyphcapacity);
		return 1;
	}
}

/** Load's glyph at 'index' in the f->ft_face->glyph slot and "render" it.
 * This is the only way to access glyph properties from freetype - by loading
 * the glyph into the glyph slot.
 * \return 0 on success
 */
static int _font_glyph_loadslot(font* f, uint index)
{
	int error = 0;
	error = FT_Load_Glyph(f->ft_face, index, FT_LOAD_DEFAULT);
	if(error) {
		printf("E: %s(index=%d): failed to load glyph.\n", __func__, index);
		return 1;
	}
	error = FT_Render_Glyph(f->ft_face->glyph, FT_RENDER_MODE_NORMAL);
	if(error) {
		printf("E: %s(index=%d): failed to render glyph.\n", __func__, index);
		return 1;
	}
	return 0;
}

/** Cache all the glyph data */
static void _font_loadglyphs(font* f)
{
	_font_glyph_reserve(f, (FNTDEF_ASCIIEND - FNTDEF_ASCIIBEGIN));

	int count = 0;
	for(int i = FNTDEF_ASCIIBEGIN; i < FNTDEF_ASCIIEND; ++i) {
		int gindex = FT_Get_Char_Index(f->ft_face, (char)i);
		if(gindex) {
			// Load the glyph into slot
			_font_glyph_loadslot(f, gindex);
			// Aliases
			FT_GlyphSlot slot = f->ft_face->glyph;
			FT_Bitmap bitmap = slot->bitmap;

			// Cache glyph
			glyph g = {
				g.index = gindex,
				g.ch = (char) i,
				{ // g.advance substruct init
					g.advance.x = slot->advance.x,
					g.advance.y = slot->advance.y
				},
				g.bitmap_top = slot->bitmap_top,
				g.bitmap_left = slot->bitmap_left,
				g.bitmap_width = bitmap.width,
				g.bitmap_height = bitmap.rows,
			};

			_font_glyph_add(f, &g);
			/*font_printarb(bitmap.buffer, bitmap.width, bitmap.rows);*/
			/*printf("D: found:  %d -> %c\n", gindex, (char)i);*/
			count++;
		}
	}
	printf("Loaded %d glyphs.\n", count);
}

const glyph* font_glyph_get(const font* fnt, char ch)
{
	for(uint i = 0; i < fnt->glyphcount; ++i) {
		if(fnt->glyphs[i].ch == ch)
			return &fnt->glyphs[i];
	}
	printf("W: %s(char=%c): not found!\n", __func__, ch);
	return NULL;
}


font* font_load(const char* path)
{
	_check_init();
	font* f = malloc(sizeof(font));
	_init_font(f);

	// Load font
	int error = 0;
	error = FT_New_Face(FTLIB, path, 0, &f->ft_face);
	if(error == FT_Err_Unknown_File_Format) {
		printf("E: %s: failed to load font face: UNKNOWN FORMAT\
			   	'%s'\n", __func__, path);
		exit(1);
	} else if (error) {
		printf("E: %s: failed to load font face: '%s'\n", __func__, path);
		exit(1);
	}
	assert(f->ft_face);


	// Set size
	font_set_size(f, FNTDEF_SIZE);
	// Load ascii glyphs
	_font_loadglyphs(f);

	flag_set(&f->flags, f_init);
	return f;
}


void font_destroy(font* f)
{
	assert(f);
	// TODO
	free(f);
	f = NULL;
}


int font_set_size(font* font, uint size)
{
	int error = 0;
	error = FT_Set_Char_Size(font->ft_face,
			0, size * 64,	// width same as height(=size*64)
			0, FNTDEF_RES);	// horiz resolution same as vert(=FNTDEF_RES)

	if(error) {
		printf("E: %s(size=%d): error!\n", __func__, size);
		exit(1);
	}
	return 0;
}

void font_pprint(const font* fnt)
{
	assert(fnt);
	// TODO
}

void font_glyph_pprint(const glyph* g)
{
	assert(g);
	printf("[[Glyph]] ");
	printf("index: %d, ", g->index);
	printf("ch: '%c', ", g->ch);
	printf("bitmap_left: %d, ", g->bitmap_left);
	printf("bitmap_top: %d, ", g->bitmap_top);
	printf("[[/Glyph]]\n");
}

void font_printarb(unsigned char* buffer, uint width, uint rows)
{
	for(uint row = 0; row < rows; ++row) {
		printf("\t");
		for(uint col = 0; col < width; ++col) {
			char c;
			int d = buffer[row*width + col];
			if( d > 200 ) c = '#';
			else if (d > 40) c = 'o';
			else c = ' ';

			printf("%c", c);
		}
		printf("\n");
	}
}

