#include <SDL/SDL.h>
#include "opengl.h"

static volatile uint32_t tick_timer = 0;


static uint32_t timer_function(uint32_t i)
{
	tick_timer++;
	return i;
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

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (SDL_SetVideoMode(width, height, bpp, SDL_OPENGL) == 0) {
		fprintf(stderr, "error: %s\n", SDL_GetError());
		return -1;
	}

	tick_timer = 0;
	SDL_SetTimer(4, timer_function);
	//SDL_WM_SetCaption("");

	return init_opengl(width, height);
}

void swap_buffers()
{
	SDL_GL_SwapBuffers();
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
