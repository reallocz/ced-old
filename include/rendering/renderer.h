#pragma once
/** \file renderer.h
 * \brief Rendering functions.
 * Rather than creating a generic 'struct renderable' for calling rdr_*
 * functions, we implement a new function for each kind of 'thing' that
 * needs to be rendered. This would make the renderer monolithic, which would
 * allow for rapid changes and easier refactoring because all the rendering
 * code lives in renderer.c
 */

#include "mytypes.h"
#include "document/line.h"
#include "gui/gl/shader.h"


int rdr_initmodule();

/** Render a line.
 * \defined line_renderer.c*/
void rdr_render_line(const line* ln);

/** example api */
//void rdr_render_toolbar();
//void rdr_render_cursor();
