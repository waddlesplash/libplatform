/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "platform/events.h"
#include "platform/types.h"
#include "platform/glwindow.h"

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>

Display* display;
struct GLXWindow {
	Window windowHandle;
	Size windowSize;
	XVisualInfo* visualInfo;
	GLXContext glContext;
	Colormap colormap;
};

GLWindow glwindow_create(const char* title, Size size)
{
	struct GLXWindow* wind = malloc(sizeof(struct GLXWindow));
	if (!(display = XOpenDisplay(NULL))) {
		fprintf(stderr, "GLXWindow: could not open display!\n");
		return NULL;
	}

	int dummy;
	if (!glXQueryExtension(display, &dummy, &dummy)) {
		fprintf(stderr, "GLXWindow: no GLX present on server!\n");
		return NULL;
	}

	int attributes[] = { GLX_RGBA,
						 GLX_DOUBLEBUFFER,
						 GLX_RED_SIZE, 8,
						 GLX_GREEN_SIZE, 8,
						 GLX_BLUE_SIZE, 8,
						 GLX_DEPTH_SIZE, 8,
						 0 };
	if (!(wind->visualInfo = glXChooseVisual(display, DefaultScreen(display),
		attributes))) {
		fprintf(stderr, "GLXWindow: no RGB visual with depth buffer!\n");
		return NULL;
	}

	if (!(wind->glContext = glXCreateContext(display, wind->visualInfo, None,
		True))) {
		fprintf(stderr, "GLXWindow: could not create an OpenGL context!\n");
		return NULL;
	}
	wind->colormap = XCreateColormap(display,
		RootWindow(display, wind->visualInfo->screen),
		wind->visualInfo->visual, AllocNone);

	XSetWindowAttributes windowAttributes;
	windowAttributes.colormap = wind->colormap;
	windowAttributes.border_pixel = 0;
	windowAttributes.event_mask = ExposureMask | StructureNotifyMask |
		ButtonPressMask | KeyPressMask | KeymapStateMask;

	wind->windowHandle =
		XCreateWindow(display,
			RootWindow(display, wind->visualInfo->screen),
			0, 0, size.width, size.height,
			0, // Borderwidth
			wind->visualInfo->depth, // Depth
			InputOutput, wind->visualInfo->visual,
			CWBorderPixel | CWColormap | CWEventMask,
			&windowAttributes);
	wind->windowSize = size;
	XSetStandardProperties(display, wind->windowHandle, title, NULL,
		None, 0, 0, NULL);

	glXMakeCurrent(display, wind->windowHandle, wind->glContext);
	XMapWindow(display, wind->windowHandle);

	return (GLWindow)wind;
}

void glwindow_destroy(GLWindow window)
{
	struct GLXWindow* wind = (struct GLXWindow*)window;
	XUnmapWindow(display, wind->windowHandle);
	XDestroyWindow(display, wind->windowHandle);
	glXMakeCurrent(display, None, NULL);
	glXDestroyContext(display, wind->glContext);
	XFreeColormap(display, wind->colormap);
	XFree(wind->visualInfo);
	XCloseDisplay(display);
	free(wind);
}

void glwindow_flush(GLWindow window)
{
	struct GLXWindow* wind = (struct GLXWindow*)window;
	glXSwapBuffers(display, wind->windowHandle);
}

int32_t glwindow_get_event(GLWindow window)
{
	struct GLXWindow* wind = (struct GLXWindow*)window;
	if (XPending(display) == 0)
		return EVENT_NONE;

	XEvent event;
next:
	XNextEvent(display, &event);
	switch(event.type) {
	case KeymapNotify:
		XRefreshKeyboardMapping(&event.xmapping);
	break;
	case KeyPress:
		return event.xkey.keycode;
	break;
	case ConfigureNotify:
		if (wind->windowSize.width == event.xconfigure.width
		    && wind->windowSize.height == event.xconfigure.height)
			goto next;
		wind->windowSize.width = event.xconfigure.width;
		wind->windowSize.height = event.xconfigure.height;
		return EVENT_RESIZE;
	break;
	default:
		// We don't care about this event
		goto next;
	break;
	}

	return EVENT_NONE; // We might want an assert here...
}

Size glwindow_get_size(GLWindow window)
{
	struct GLXWindow* wind = (struct GLXWindow*)window;
	return wind->windowSize;
}
