#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include "u2gl.h"

static volatile uint32_t tick_timer = 0;
static EGLDisplay display;
static EGLContext context;
static EGLSurface surface;
static Display *x11Disp = NULL;

#define configs_in 10

int window_width = 640;
int window_height = 480;

static uint32_t timer_function(uint32_t i)
{
	tick_timer++;
	return i;
}

static void close_window()
{
	if (display) {
		eglMakeCurrent(display, NULL, NULL, EGL_NO_CONTEXT);
		if (context)
			eglDestroyContext(display, context);
		if (surface)
			eglDestroySurface(display, surface);
		eglTerminate(display);

		surface = 0;
		context = 0;
		display = 0;
	}

	if (x11Disp)
		XCloseDisplay(x11Disp);

	x11Disp = NULL;
}

static EGLint s_configAttribs[] = {
	EGL_RED_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_BLUE_SIZE, 8,
	EGL_DEPTH_SIZE, 24,
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	EGL_NONE
};

static EGLint contextParams[] = {
	EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
};


int init_graphics(char *caption, int width, int height)
{
	const SDL_VideoInfo *info;
	int bpp;
	EGLint num_configs = 0;
	EGLConfig configs[configs_in];
	EGLConfig my_config;
	SDL_SysWMinfo sysInfo;

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

	display = eglGetDisplay((EGLNativeDisplayType) x11Disp);
	if (display == EGL_NO_DISPLAY) {
		fprintf(stderr, "Failed to get EGL display");
		close_window();
		return -1;
	}

	if (!eglInitialize(display, NULL, NULL)) {
		fprintf(stderr, "Failed to initialize EGL display");
		close_window();
		return -1;
	}

	/* Choose configuration */
	if (eglChooseConfig(display, s_configAttribs, configs,
	    configs_in, &num_configs) != EGL_TRUE || num_configs == 0) {
		close_window();
		fprintf(stderr, "error: No suitable EGL configuration");
		return -1;
	}

	/* Bind GLES API */
	my_config = configs[0];
	eglBindAPI(EGL_OPENGL_ES_API);

	/* Create EGL Context */
	context = eglCreateContext(display, my_config, EGL_NO_CONTEXT,
			     contextParams);
	if (context == EGL_NO_CONTEXT) {
		close_window();
		fprintf(stderr, "error: Failed to create EGL context\n");
		return -1;
	}

	/* Get window manager information */
	SDL_VERSION(&sysInfo.version);
	if (SDL_GetWMInfo(&sysInfo) <= 0) {
		close_window();
		fprintf(stderr, "error: SDL_GetWMInfo failed\n");
		return -1;
	}

	/* Create surface */
	surface = eglCreateWindowSurface(display, my_config,
			(EGLNativeWindowType)sysInfo.info.x11.window, NULL);
	if (surface == EGL_NO_SURFACE) {
		close_window();
		fprintf(stderr, "error: Failed to create EGL surface\n");
		return -1;
	}

	/* Make EGL current */
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		close_window();
		fprintf(stderr, "error: Failed to make EGL current\n");
		return -1;
	}

	tick_timer = 0;
	SDL_SetTimer(4, timer_function);
	SDL_WM_SetCaption(caption, caption);

	return 0;
}

void swap_buffers()
{
	eglSwapBuffers(display, surface);
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
