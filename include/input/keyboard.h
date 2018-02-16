#pragma once
#include "gui/window.h"

typedef struct kbdkey {
	int key;
	int action;
	int mods;
} kbdkey;


/** Return's the type of doccmd given the key
 * \see http://www.glfw.org/docs/latest/group__keys.html
 */
enum doccmd_type kbd_gettype(kbdkey ik);
