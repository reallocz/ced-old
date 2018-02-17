#include "gui/window.h"
#include <stdio.h>
#include <stdlib.h>
#include "ds/strbuf.h"
#include "document/line.h"
#include "gui/gl/shader.h"
#include "config.h"
#include "fileutils.h"
#include "font/font.h"

int init(void)
{
	int err = 0;
	err = win_initmodule();
	if(err) return err;
	err = font_initmodule();
	if(err) return err;
	return 0;
}

/* run once and and exit */
void test()
{
	font* f = font_load(CONF_FONT_ROOT "/mono.ttf");
	const glyph* res = font_glyph_get(f, 'b');
	font_glyph_pprint(res);
	exit(0);
}

int main(void)
{

	if(init() != 0) {
		printf("E: %s: failed to init one of the modules\n", __func__);
		exit(1);
	}

	test(); // FIXME

	font* f = font_load(CONF_FONT_ROOT "/mono.ttf");
	document d = doc_createfrom_file(sb_createfrom_str("../tmp/file"));

	window* win = win_create(640, 480);
	win_setdoc(win, &d);

	/*shader sh = shader_create(CONF_RES_ROOT "/shaders/vert.s", CONF_RES_ROOT "/shaders/frag.s");*/
	/*shader_use(&sh);*/

	/* Main loop */
	while (!win_shouldclose(win))
	{
		/**
		 * Rendering
		 * window: clear->update->pollevents cycle
		 */
		win_clear(win);
		win_update(win);
		win_waitevents();
	}

	win_destroy(win);
	return 0;
}
