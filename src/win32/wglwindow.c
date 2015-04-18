/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "platform/events.h"
#include "platform/types.h"
#include "platform/glwindow.h"

#include <windows.h>

#include <stdlib.h>

#define WGLWindowClass "WGLWindow"
struct WGLWindow {
	HWND hWnd;
	Size hWndSize;
	HDC hDC;
	HPALETTE hPalette;
	HGLRC hGLRC;
};
LRESULT APIENTRY WGLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GLWindow glwindow_create(const char* title, Size size)
{
	struct WGLWindow* wind = malloc(sizeof(struct WGLWindow));
	HINSTANCE hCurrentInst = GetModuleHandle(NULL);

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WGLWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hCurrentInst;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WGLWindowClass;
	RegisterClass(&wndClass);

	wind->hWnd = CreateWindow(WGLWindowClass, title,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, size.width, size.height, NULL, NULL, hCurrentInst, NULL);
	wind->hWndSize = size;

	ShowWindow(wind->hWnd, SW_SHOW);
	UpdateWindow(wind->hWnd);
	wind->hDC = GetDC(wind->hWnd);

	// Create the pixel format
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), /* size */
		1,					/* version */
		PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,		/* color type */
		16,					/* prefered color depth */
		0, 0, 0, 0, 0, 0,	/* color bits (ignored) */
		0,					/* no alpha buffer */
		0,					/* alpha bits (ignored) */
		0,					/* no accumulation buffer */
		0, 0, 0, 0,			/* accum bits (ignored) */
		16,					/* depth buffer */
		0,					/* no stencil buffer */
		0,					/* no auxiliary buffers */
		PFD_MAIN_PLANE,		/* main layer */
		0,					/* reserved */
		0, 0, 0				/* no layer, visible, damage masks */
	};
	int pixelFormat;

	pixelFormat = ChoosePixelFormat(wind->hDC, &pfd);
	if (pixelFormat == 0) {
		MessageBox(wind->hWnd, "ChoosePixelFormat failed.", "Error",
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	if (SetPixelFormat(wind->hDC, pixelFormat, &pfd) != TRUE) {
		MessageBox(wind->hWnd, "SetPixelFormat failed.", "Error",
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	// Create the pallete
	if (pfd.dwFlags & PFD_NEED_PALETTE) {
		LOGPALETTE* pPal;
		int paletteSize;

		paletteSize = 1 << pfd.cColorBits;
		pPal = (LOGPALETTE*)
			malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
		pPal->palVersion = 0x300;
		pPal->palNumEntries = paletteSize;

		int redMask = (1 << pfd.cRedBits) - 1;
		int greenMask = (1 << pfd.cGreenBits) - 1;
		int blueMask = (1 << pfd.cBlueBits) - 1;

		for (int i = 0; i < paletteSize; ++i) {
			pPal->palPalEntry[i].peRed =
				(((i >> pfd.cRedShift) & redMask) * 255) / redMask;
			pPal->palPalEntry[i].peGreen =
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
			pPal->palPalEntry[i].peBlue =
				(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
			pPal->palPalEntry[i].peFlags = 0;
		}

		wind->hPalette = CreatePalette(pPal);
		free(pPal);

		if (wind->hPalette != NULL) {
			SelectPalette(wind->hDC, wind->hPalette, FALSE);
			RealizePalette(wind->hDC);
		}
	}

	wind->hGLRC = wglCreateContext(wind->hDC);
	wglMakeCurrent(wind->hDC, wind->hGLRC);

	return (GLWindow)wind;
}

void glwindow_destroy(GLWindow window)
{
	struct WGLWindow* wind = (struct WGLWindow*)window;
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(wind->hGLRC);
	ReleaseDC(wind->hWnd, wind->hDC);
	DestroyWindow(wind->hWnd);
	free(wind);
}

void glwindow_flush(GLWindow window)
{
	struct WGLWindow* wind = (struct WGLWindow*)window;
	SwapBuffers(wind->hDC);
}

// Stuff for WGLWindowProc to use/set.
struct WGLWindow* glwindow_event_wind = NULL;
int32_t glwindow_event_retcode;
int32_t glwindow_get_event(GLWindow window)
{
	glwindow_event_wind = (struct WGLWindow*)window;
	glwindow_event_retcode = EVENT_NONE;

	MSG msg;
next:
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (glwindow_event_retcode == EVENT_NONE)
			goto next;
	}

	return glwindow_event_retcode;
}
LRESULT APIENTRY WGLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (glwindow_event_wind == NULL)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	switch (msg) {
	case WM_SIZE:
		glwindow_event_wind->hWndSize.width = (int)LOWORD(lParam);
		glwindow_event_wind->hWndSize.height = (int)HIWORD(lParam);
		glwindow_event_retcode = EVENT_RESIZE;
		return 0;

	case WM_DESTROY:
	case WM_QUIT:
		glwindow_event_retcode = EVENT_QUIT;
		return 0;

	case WM_CHAR:
		glwindow_event_retcode = (int)wParam;
	break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Size glwindow_get_size(GLWindow window)
{
	struct WGLWindow* wind = (struct WGLWindow*)window;
	return wind->hWndSize;
}
