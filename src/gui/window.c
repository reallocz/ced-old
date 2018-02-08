#include "gui/window.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define GLV_MAJOR 3
#define GLV_MINOR 3
#define WINDEF_TITLE "[NEW DOCUMENT]"


struct window_t {
	uint flags;
	GLFWwindow* gwin;
	int width;
	int height;
	strbuf title;
	int shouldclose;
};

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

static GLFWwindow* _win_create_glfwwindow(uint width, uint height, strbuf* title)
{
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(width, height, sb_getstr(title),
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

window* win_create(uint width, uint height)
{
	_win_initglfw();
	window* win = malloc(sizeof(window));
	win->flags = 0;
	win->gwin = NULL;
	win->width = width;
	win->height = height;
	win->title = sb_createfrom_str(WINDEF_TITLE);
	win->shouldclose = 0;

	win->gwin = _win_create_glfwwindow(win->width, win->height, &win->title);
	flag_set(&win->flags, f_init);
	// Set window pointer
	glfwSetWindowUserPointer(win->gwin, win);
	return win;
}


// Terminates the whole program
void win_destroy(window* win)
{
	assert(win != NULL);
	if(flag_isset(win->flags, f_dead)) {
		printf("E: %s: Double destroy.\n", __func__);
		return;
	}
	sb_destroy(&win->title);

	// Glfw stuff
	glfwDestroyWindow(win->gwin);
	glfwTerminate();

	// Set flags
	flag_unset(&win->flags, f_init);
	flag_set(&win->flags, f_dead);
	win = NULL;
	printf("%s: window destoryed.\n", __func__);
}

void win_settitle(window* win, strbuf title)
{
	win->title = title;
	glfwSetWindowTitle(win->gwin, sb_getstr(&title));
}

void win_update(window* win)
{
	GLFWwindow* gwin = win->gwin;

	// Check for close flags
	if(win->shouldclose == 1) {
		printf("%s: window close flag set.\n", __func__);
		glfwSetWindowShouldClose(gwin, GLFW_TRUE);
	}
	// Update window and framebuffer sizes
	{
		int width, height;
		glfwGetWindowSize(gwin, &width, &height);
		win->width = width;
		win->height = height;
		glViewport(0, 0, width, height);
	}

	// Swap buffers
	glfwSwapBuffers(win->gwin);
}


void win_clear(window* win)
{
	glClearColor(255, 255, 255, 255);
	glClear(GL_COLOR_BUFFER_BIT);
}


// Makes the thread sleep and resumes it on events
void win_waitevents()
{
	glfwWaitEvents();
}


void win_setclose(window* win, int val)
{
	win->shouldclose = 1;
}

int win_shouldclose(const window* win)
{
	return win->shouldclose || glfwWindowShouldClose(win->gwin);
}

