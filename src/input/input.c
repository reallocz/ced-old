#include "input/input.h"
#include "input/keyboard.h"
#include <stdio.h>

void inp_onkey(window* win, int key, int action, int mods)
{
	kbdkey kk = {.key=key, .action=action, .mods=mods};
	if(!win_getdoc(win)) {
		printf("No document set!\n");
		return;
	}

	/* ignore key releases for now */
	if(kk.action == GLFW_RELEASE)
		return;

	if(kk.key == GLFW_KEY_ESCAPE) {
		win_setclose(win, 1);
		return;
	}

	document* doc = win_getdoc(win);
	doccmd cmd = dc_create(kbd_gettype(kk));

	/* Dispatch to doc_exec */
	switch (cmd.type) {
		case (INSERT):
			cmd.data.c = kk.key;
			doc_exec(doc, cmd);
			break;
		case (EDIT):
			doc_exec(doc, cmd);
		default:
			break;
	}
}

