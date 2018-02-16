#include "input/input.h"
#include "document/doccmd.h"
#include <stdio.h>
#include <assert.h>

// macros

/** Add GLFW_KEY_ prefix */
#define GKEY(K) GLFW_KEY_##K
/** Compare key with GKEY. *NOTE*: 'key' must be in scope! */
#define KEYIS(K) (key == GKEY(K))

/** Get the type of command from the key
 * \see http://www.glfw.org/docs/latest/group__keys.html
 */
enum doccmd_type _inp_gettype(inpkey ik)
{
	int key = ik.key; // alias

	// All alpha-numeric keys and some symbols
	// TODO add rest of the symbols
	if ((key >= 65 && key <= 90)			// Alpha keys
			|| (key >= 48 && key <= 57)		// Numeric (numpad) keys
			|| (key >= 320 && key <= 329)	// Numeric (keypad) keys
			|| (key >= 44 && key <= 47)		// Some symbols
			|| (key >= 91 && key <= 98)	// [, /, ]
			|| KEYIS(SPACE) || KEYIS(APOSTROPHE)	|| KEYIS(SEMICOLON)
			|| KEYIS(EQUAL) || KEYIS(TAB)
			)
	{
		return INSERT;
	}

	else if(KEYIS(BACKSPACE) || KEYIS(DELETE))
	{
		return EDIT;
	}

	else if(KEYIS(UP) || KEYIS(DOWN) || KEYIS(LEFT) || KEYIS(RIGHT)
				|| KEYIS(PAGE_UP) || KEYIS(PAGE_DOWN) || KEYIS(HOME)
				|| KEYIS(END)
			)
	{
		return MOVE;
	}

	else
	{
		return NOP;
	}
}


void inp_onkey(window* win, inpkey ik)
{
	if(!win_getdoc(win)) {
		printf("No document set!\n");
		return;
	}

	/* ignore key releases for now */
	if(ik.action == GLFW_RELEASE)
		return;

	if(ik.key == GLFW_KEY_ESCAPE) {
		win_setclose(win, 1);
		return;
	}

	document* doc = win_getdoc(win);
	doccmd cmd = dc_create(_inp_gettype(ik));

	/* Dispatch to doc_exec */
	switch (cmd.type) {
		case (INSERT):
			cmd.data.c = ik.key;
			doc_exec(doc, cmd);
			break;
		case (EDIT):
			doc_exec(doc, cmd);
		default:
			break;
	}
}

