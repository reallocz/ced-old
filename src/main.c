#include "gui/window.h"
#include <stdio.h>
#include "core/strbuf.h"
#include "core/line.h"
#include "gui/gl/shader.h"
#include "config.h"
#include "fileutils.h"


int main(void)
{
	document d = doc_createfrom_file(sb_createfrom_str("../tmp/file"));
	doc_pprint(&d);

#if 1
	/*shader_use(&sh);*/
	window* win = win_create(640, 480);
	shader sh = shader_create(CONF_RES_ROOT "/shaders/vert.s", CONF_RES_ROOT "/shaders/frag.s");

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
#endif
	return 0;
}
