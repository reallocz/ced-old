#include "gui/window.h"
#include <stdio.h>
#include "core/strbuf.h"
#include "core/line.h"

int main(void)
{
	strbuf a = sb_createfrom_str("Hello, World!");
	sb_pprint(&a);
	window win = win_create(640, 480);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(win.gwin))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(win.gwin);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
