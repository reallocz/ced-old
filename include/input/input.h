#pragma once
#include "gui/window.h"
#include "input/keyboard.h"

/** Primary window key event callback. */
void inp_onkey(window* win, int key, int action, int mods);

// TODO
void inp_onmouse_movement(window* win);
void inp_onmouse_button(window* win);

// TODO sys signal handling
