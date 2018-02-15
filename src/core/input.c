#include "core/input.h"
#include "core/doccmd.h"
#include <stdio.h>
#include <assert.h>


/** Get the type of command from the key */
enum doccmd_type _inp_gettype(inpkey ik)
{
	int key = ik.key; // alias

	if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
		return INSERT;
	} else {
		return NOP;
	}
}


void inp_onkey(window* win, inpkey ik)
{
	/* ignore key releases for now */
	if(!win_getdoc(win)) {
		printf("No document set!\n");
		return;
	}
	if(ik.action == GLFW_RELEASE)
		return;

	if(ik.key == GLFW_KEY_ESCAPE) {
		win_setclose(win, 1);
		return;
	}

	enum doccmd_type type = _inp_gettype(ik);
	document* doc = win_getdoc(win);
	doccmd cmd = dc_create(type);

	/* Dispatch to doc_exec */
	switch (type) {
		case (INSERT):
			cmd.data.c = ik.key;
			doc_exec(doc, cmd);
			break;
		default:
			break;
	}
}

