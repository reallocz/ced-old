#pragma once
#include "gui/window.h"

typedef struct inpkey {
	int key;
	int action;
	int mods;
} inpkey;

/** Primary window key callback.*/
void inp_onkey(window* win, inpkey key);

