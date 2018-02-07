#include "gui/window.h"
#include <stdio.h>
#include "core/strbuf.h"
#include "core/line.h"

int main(void)
{
	strbuf a = sb_createfrom_str("Hello, World!");
	sb_pprint(&a);
	window win = win_create(640, 480);

	/* Main loop */
	while (!win_shouldclose(&win))
	{
		/** 
		 * Rendering
		 * window: clear->update->pollevents cycle
		 */
		win_clear(&win);
		win_update(&win);
		win_pollevents(&win);
	}

	win_destroy(&win);
	return 0;
}
