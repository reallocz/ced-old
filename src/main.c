#include "gui/window.h"
#include <stdio.h>
#include <stdlib.h>
#include "ds/strbuf.h"
#include "document/line.h"
#include "gui/gl/shader.h"
#include "config.h"
#include "fileutils.h"
#include "font/font.h"
#include "rendering/renderer.h"

int init(void)
{
	int err = 0;
	err = win_initmodule();
	if(err) return err;
	err = font_initmodule();
	if(err) return err;
	err = rdr_initmodule();
	if(err) return err;
	return 0;
}

void test()
{
	float verts[] = {
		0, 0, 1,
		0, 1, 0.5,
		1, 0, 0,
	};
	GLuint VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAO);
}

int main(void)
{

	if(init() != 0) {
		printf("E: %s: failed to init one of the modules\n", __func__);
		exit(1);
	}


	font* f = font_load(CONF_FONT_ROOT "/mono.ttf");
	document d = doc_createfrom_file(sb_createfrom_str("../tmp/file"));

	window* win = win_create(640, 480);
	win_setdoc(win, &d);

	test(); // FIXME
	shader sh = shader_create(CONF_RES_ROOT "/shaders/vert.s", CONF_RES_ROOT "/shaders/frag.s");

	/* Main loop */
	while (!win_shouldclose(win))
	{
		/**
		 * Rendering
		 * window: clear->update->pollevents cycle
		 */
		win_clear(win);

		// Draw start
		for(uint i = 0; i < d.numlines; ++i) {
			rdr_render_line(&d.lines[i]);
		}
		shader_use(&sh);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw end

		win_update(win);
		win_waitevents();
	}

	win_destroy(win);
	return 0;
}
