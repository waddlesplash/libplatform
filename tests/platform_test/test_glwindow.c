/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "testutils.h"

#include "platform/glwindow.h"

#include <stddef.h>

void run_glwindow_tests()
{
	GLWindow w = NULL;
	w = glwindow_create("Test", (Size){640, 480});
	test_result((w != NULL), "glwindow_create()");
	// TODO: draw something & read pixels
	Size s = glwindow_get_size(w);
	test_result((s.width == 640) && (s.height == 480), "glwindow_get_size()");
	glwindow_destroy(w);
}
