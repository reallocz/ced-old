#include "gui/window.h"
#include <stdio.h>
#include "ds/strbuf.h"
#include "document/line.h"
#include "gui/gl/shader.h"
#include "config.h"
#include "fileutils.h"


int main(void)
{
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
