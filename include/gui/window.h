#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mytypes.h"
#include "core/strbuf.h"

typedef struct window_t window;

/** window constructor */
window* win_create(uint width, uint height);

/** window destructor */
void win_destroy(window* win);

void win_settitle(window* win, const char* title);
void win_update(window* win);
void win_clear(window* win);
void win_pollevents(window* win);
void win_setclose(window* win, int val);
int win_shouldclose(const window* win);

