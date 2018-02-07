#include "gui/window.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define GLV_MAJOR 3
#define GLV_MINOR 3
#define WINDEF_TITLE "[NEW DOCUMENT]"

static void _win_glfw_onerror(int error, const char* desc)
{
	printf("E: %s: code=%d: %s.\n", __func__, error, desc);
}

static void _win_initglfw()
{
	/* Initialize the library */
	if(! glfwInit()) {
		printf("E: %s: Failed to init GLFW.\n", __func__);
		exit(1);
	}
	glfwSetErrorCallback(_win_glfw_onerror);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLV_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLV_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static GLFWwindow* _win_create_glfwwindow(uint width, uint height)
{
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(width, height, WINDEF_TITLE,
			NULL, NULL);
	if (!window)
	{
		printf("E: %s: Failed to create glfw window.\n", __func__);
		glfwTerminate();
		exit(1);
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("E: %s:Failed to init glad!\n", __func__);	
		exit(1);
	}
	return window;
}

window win_create(uint width, uint height)
{
	_win_initglfw();
	window win = {
		.flags = 0,
		.gwin = NULL,
		.width = width,
		.height = height,
	};

	win.gwin = _win_create_glfwwindow(win.width, win.height);
	flag_set(&win.flags, f_init);
	return win;
}


void win_destroy(window* win)
{
	assert(win != NULL);
	if(flag_isset(win->flags, f_dead)) {
		printf("E: %s: Double destroy.\n", __func__);
		return;
	}
	glfwDestroyWindow(win->gwin);
	flag_unset(&win->flags, f_init);
	flag_set(&win->flags, f_dead);
	win = NULL;
}

void win_settitle(window* win, const char* title)
{
	glfwSetWindowTitle(win->gwin, title);
}



