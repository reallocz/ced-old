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

struct font_t {
	uint flags;
	FT_Face ft_face;
	uint size;
	uint resolution;
};

/* Init only once */
static FT_Library g_ftlib = NULL;

/** Initialize freetype2.
 * Freetype2 must be initialized before using any of the font_ functions.
 */
static int _font_initfreetype()
{
	if(g_ftlib) {
		printf("E: %s: double init!\n", __func__);
		return 1;
	}
	int error = 0;
	error = FT_Init_FreeType(&g_ftlib);
	if(error) {
		printf("E: %s: failed to init freetype!\n", __func__);
		return 1;
	}
	assert(g_ftlib);
	return 0;
}

int font_initmodule()
{
	return _font_initfreetype();
}

/** Exit if freetype's not init */
static void _check_init()
{
	if(! g_ftlib) {
		printf("E: %s: freetype is not initialized!\n", __func__);
		exit(1);
	}
}

static void _init_font(font* f)
{
	f->flags = 0;
	f->ft_face = NULL;
	f->size = 0;
	f->resolution = 0;
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

font* font_load(const char* path)
{
	_check_init();
	font* f = malloc(sizeof(font));
	_init_font(f);

	// Load font
	int error = 0;
	error = FT_New_Face(g_ftlib, path, 0, &f->ft_face);
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


