#include "gui/window.h"
#include <stdio.h>
#include "core/strbuf.h"
#include "core/line.h"

int main(void)
{
	strbuf a = sb_createfrom_str("Hello, World!");
	sb_pprint(&a);
	window* win = win_create(640, 480);

	double time = glfwGetTime();
	/* Main loop */
	while (!win_shouldclose(win))
	{
		/** 
		 * Rendering
		 * window: clear->update->pollevents cycle
		 */
		printf("diff = %f\n", glfwGetTime() - time);
		time = glfwGetTime();
		win_clear(win);
		win_update(win);
		win_waitevents();
	}

	win_destroy(win);
	return 0;
}
