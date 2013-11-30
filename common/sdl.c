#include <stdio.h>
#include <sys/time.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <getopt.h>
#include "u2gl.h"

static volatile uint32_t tick_timer = 0;
static EGLDisplay display;
static EGLContext context;
static EGLSurface surface;
static Display *x11_display = NULL;

#define MAX_CONFIGS 10

int window_width = 640;
int window_height = 480;
static int frame_dump = 0;

static void dump_frame(void);

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

	if (x11_display)
		XCloseDisplay(x11_display);

	x11_display = NULL;
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

extern char *optarg;

#define OPTIONS "ds:"
static int parse_options(int argc, char **argv)
{
	int o;

	while ((o = getopt(argc, argv, OPTIONS)) != -1) {
		switch (o) {
		case 'd':
			frame_dump = 1;
			break;
		case 's':
			if ((strtok(optarg, "x"))) {
				window_width = atoi(optarg);
				window_height = atoi(strtok(NULL, ""));
				printf("window size: %dx%d\n",
						window_width, window_height);
			}
			break;
		default:
			return -1;
		}
	}

	return 0;
}

int init_graphics(char *caption, int argc, char **argv)
{
	const SDL_VideoInfo *info;
	int bpp;
	EGLint num_configs = 0;
	EGLConfig configs[MAX_CONFIGS];
	EGLConfig my_config;
	SDL_SysWMinfo sysInfo;

	if (parse_options(argc, argv) < 0)
		return -1;

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

	if (SDL_SetVideoMode(window_width, window_height, bpp, SDL_OPENGL) == 0) {
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return -1;
	}

	x11_display = XOpenDisplay(0);
	if (x11_display == NULL) {
		fprintf(stderr, "Failed to open X display");
		return -1;
	}

	display = eglGetDisplay((EGLNativeDisplayType) x11_display);
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
	    MAX_CONFIGS, &num_configs) != EGL_TRUE || num_configs == 0) {
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

static long frameus = 1000000 / 70;
static long deltaus;

void set_fps(float fps)
{
	frameus = 1000000 / fps;
}

void swap_buffers()
{
	static long frames, startus, oldus = -1;
	struct timeval tv;
	long us;
	float fps, avg;

	gettimeofday(&tv, NULL);
	us = tv.tv_sec * 1000000 + tv.tv_usec;

	if (oldus < 0) {
		startus = oldus = us;
		frames = 0;
	}

	deltaus = us - oldus;
	if (frame_dump) {
		deltaus = 1000000 / 60;
	}
	oldus = us;

	if (deltaus > 0) {
		fps = 1000000.0 / deltaus;
		avg = 1000.0 * frames / ((us - startus) / 1000.0);
	} else {
		avg = fps = 0.0;
	}

	frames++;

	printf("fps = %5.1f, avg = %5.1f    \r", fps, avg);
	
	if (frame_dump)
		dump_frame();
	eglSwapBuffers(display, surface);
}

int adjust_framerate()
{
	static long adjust = 0;
	int num_frames;

	adjust += deltaus;

	num_frames = adjust / frameus;
	//printf("deltaus=%ld  frameus=%ld  adjust=%ld  num_frames=%d\n", deltaus, frameus, adjust, num_frames);

	adjust -= frameus * num_frames;

	return num_frames;
}

#if 0
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
#endif

void poll_event()
{
	SDL_Event event;

	//timer_delay();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			break;
		case SDL_QUIT:
			break;
		}
	}
}

static void dump_frame()
{
	static char *buffer = NULL;
	static int num;
	char name[80], *src, *dest;
	FILE *f;
	int i;

	if (buffer == NULL) {
		//glReadBuffer(GL_BACK);
		buffer = malloc(4 * window_width * window_height);
		num = 0;
	}

	glReadPixels(0, 0, window_width, window_height, GL_RGBA,
						GL_UNSIGNED_BYTE, buffer);
	u2gl_check_error("glReadPixels");

	snprintf(name, 80, "frame_%06d.raw", num++);
	f = fopen(name, "wb");
	if (f == NULL)
		return;
	
	int bytes_in_row = window_width * 4;
	int bytes_left = window_width * window_height * 4;
	while (bytes_left > 0) {
		int start_of_row = bytes_left - bytes_in_row;

		src = dest = &buffer[start_of_row];

		for (i = 0; i < window_width; i++) {
			unsigned char r = *src++;
			unsigned char g = *src++;
			unsigned char b = *src++;
			src++;

			*dest++ = r;
			*dest++ = g;
			*dest++ = b;
		}

		fwrite (&buffer[start_of_row], 1, window_width * 3, f);
		bytes_left -= bytes_in_row;
	}
	fclose (f);
}

