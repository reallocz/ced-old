#include "core/input.h"
#include "core/doccmd.h"
#include <stdio.h>
#include <assert.h>


void _inp_quit(window* win, int val)
{
	win_setclose(win, val);
}

enum doccmd_type _inp_gettype(inpkey ik)
{
	int key = ik.key; // alias

	if(key == GLFW_KEY_ESCAPE) {
		return QUIT;
	} else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
		return INSERT;
	} else {
		return NOP;
	}
}


void inp_onkey(window* win, inpkey ik)
{
	/* ignore key releases for now */
	if(ik.action == GLFW_RELEASE)
		return;

	enum doccmd_type type = _inp_gettype(ik);
	document* doc = win_getdoc(win);
	doccmd cmd;
	switch (type) {
		case (INSERT):
			cmd = dc_create(INSERT);
			doc_exec(doc, cmd);
			break;
		case (QUIT):
			_inp_quit(win, 1);
			break;
		default:
			break;
	}
}

