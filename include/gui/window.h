#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mytypes.h"
#include "core/strbuf.h"

typedef struct window {
	uint flags;
	GLFWwindow* gwin;
	uint width;
	uint height;
	strbuf title;
} window;


// Create a new window
window win_create(uint width, uint height);
void win_destroy(window* win);
void win_settitle(window* win, const char* title);

