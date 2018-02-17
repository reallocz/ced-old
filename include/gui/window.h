#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mytypes.h"
#include "ds/strbuf.h"
#include "document/document.h"

typedef struct window_t window;

/** Initializes glfw, creates gl context, and init's glad.
 * \return 0 on success
 */
int win_initmodule();


/** Standard window constructor.
 * \param width width in px
 * \param height height in px
 * \return handle to the window
 */
window* win_create(uint width, uint height);

/** Standard window destructor.
 * Terminate's program and glfw
 */
void win_destroy(window* win);

/** Set the title of the window. */
void win_settitle(window* win, strbuf title);

/** Swap the window buffer.
 * This function's primary objective is to swap the buffer after all the
 * rendering is done. It also manages some window flags.
 */
void win_update(window* win);

/** Clear the window buffer. */
void win_clear(window* win);

/** Sleep till we get a new event. */
void win_waitevents();

/** Set the window close flag.
 * Note that it is not guranteed that the window will be closed necessarily
 * as any other part of the code can set the flag to 0.
 * \param val 1 for closing window, 0 otherwise.
 */
void win_setclose(window* win, int val);

/** Return 1 if window close flag was set.
 * The flag could've been set by `win_setclose` of by the window manager.
 * \return 1 if the close flag is set.
 */
int win_shouldclose(const window* win);

/** Set the document on the window. */
void win_setdoc(window* win, document* doc);

document* win_getdoc(const window* win);

