/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PLATFORM_GLWINDOW_H
#define PLATFORM_GLWINDOW_H

#include "events.h"
#include "geometry.h"

typedef void* GLWindow;

GLWindow glwindow_create(const char* title, Size size);
void glwindow_destroy(GLWindow window);

void glwindow_flush(GLWindow window);
int32_t glwindow_get_event(GLWindow window);

Size glwindow_get_size(GLWindow window);

#endif // PLATFORM_GLWINDOW_H
