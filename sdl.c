#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include "opengl.h"

static volatile uint32_t tick_timer = 0;
static EGLDisplay g_eglDisplay;
static EGLContext g_eglContext;
static EGLSurface g_eglSurface;
static Display *x11Disp = NULL;

#define g_totalConfigsIn 10


static uint32_t timer_function(uint32_t i)
{
	tick_timer++;
	return i;
}

static void close_window()
{
	if (g_eglDisplay) {
		eglMakeCurrent(g_eglDisplay, NULL, NULL, EGL_NO_CONTEXT);
		if (g_eglContext)
			eglDestroyContext(g_eglDisplay, g_eglContext);
		if (g_eglSurface)
			eglDestroySurface(g_eglDisplay, g_eglSurface);
		eglTerminate(g_eglDisplay);

		g_eglSurface = 0;
		g_eglContext = 0;
		g_eglDisplay = 0;
	}

	if (x11Disp)
		XCloseDisplay(x11Disp);
	x11Disp = NULL;
}

int init_graphics(int width, int height)
{
	const SDL_VideoInfo *info;
	int bpp;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return -1;
	}

	atexit(SDL_Quit);

	info = SDL_GetVideoInfo();
	if (!info) {
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return -1;
	}

	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_SetVideoMode(width, height, bpp, SDL_OPENGL) == 0) {
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return -1;
	}

	x11Disp = XOpenDisplay(0);
	if (x11Disp == NULL) {
		fprintf(stderr, "Failed to open X display");
		return -1;
	}

	g_eglDisplay = eglGetDisplay((EGLNativeDisplayType) x11Disp);
	if (g_eglDisplay == EGL_NO_DISPLAY) {
		fprintf(stderr, "Failed to get EGL display");
		close_window();
		return -1;
	}

	if (!eglInitialize(g_eglDisplay, NULL, NULL)) {
		fprintf(stderr, "Failed to initialize EGL display");
		close_window();
		return -1;
	}

#if 0
	/* Create SDL Window */
	SDLWindow = SDL_SetVideoMode(width, height, bpp, 0);
	if (!SDLWindow) {
		close_window();
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return -1;
	}
#endif

	static EGLint s_configAttribs[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};

	EGLint numConfigsOut = 0;

	EGLConfig g_allConfigs[g_totalConfigsIn];
	EGLConfig g_eglConfig;

	/* Choose configuration */
	if (eglChooseConfig(g_eglDisplay, s_configAttribs, g_allConfigs,
	    g_totalConfigsIn, &numConfigsOut) != EGL_TRUE || numConfigsOut == 0) {
		close_window();
		fprintf(stderr, "error: Could not find suitable EGL configuration");
		return -1;
	}

	/* Bind GLES API */
	g_eglConfig = g_allConfigs[0];
	eglBindAPI(EGL_OPENGL_ES_API);
	EGLint contextParams[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

	/* Create EGL Context */
	g_eglContext =
	    eglCreateContext(g_eglDisplay, g_eglConfig, EGL_NO_CONTEXT,
			     contextParams);
	if (g_eglContext == EGL_NO_CONTEXT) {
		close_window();
		fprintf(stderr, "error: Failed to create EGL context\n");
		return -1;
	}

	/* Get window manager information */
	SDL_SysWMinfo sysInfo;
	SDL_VERSION(&sysInfo.version);
	if (SDL_GetWMInfo(&sysInfo) <= 0) {
		close_window();
		fprintf(stderr, "error: SDL_GetWMInfo failed\n");
		return -1;
	}

	/* Create surface */
	g_eglSurface = eglCreateWindowSurface(g_eglDisplay, g_eglConfig,
				   (EGLNativeWindowType) sysInfo.info.x11.
				   window, NULL);
	if (g_eglSurface == EGL_NO_SURFACE) {
		close_window();
		fprintf(stderr, "error: Failed to create EGL surface\n");
		return -1;
	}

	/* Make EGL current */
	if (eglMakeCurrent(g_eglDisplay, g_eglSurface, g_eglSurface,
	     g_eglContext) == EGL_FALSE) {
		close_window();
		fprintf(stderr, "error: Failed to make EGL current\n");
		return -1;
	}

	tick_timer = 0;
	SDL_SetTimer(4, timer_function);
	//SDL_WM_SetCaption("");

	return init_opengl(width, height);
}

void swap_buffers()
{
	eglSwapBuffers(g_eglDisplay, g_eglSurface);
}

static void timer_delay()
{
	static uint32_t m = 0;

	if (tick_timer < m)
		m = 0;

	while (tick_timer - m < 2) {
		SDL_Delay(1);
	}
	m = tick_timer;
}

void poll_event()
{
	SDL_Event event;

	timer_delay();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			break;
		case SDL_QUIT:
			break;
		}
	}
}
