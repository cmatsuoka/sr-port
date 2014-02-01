#include <stdio.h>
#include "../dis/dis.h"
#include "graphics.h"
#include "opengl.h"
#include "common.h"

extern void plz();
extern void vect();

int main(int argc, char **argv)  {
	dis_partstart();
	float time = 0.0;

	if (init_graphics("Plzpart", argc, argv) < 0) {
		fprintf(stderr, "Can't init graphics\n");
		return -1;
	};

	init_opengl();
	//init_copper();
	//initvect();

	while(!dis_exit() && time < 32.32) {
		draw_bg(time);
		swap_buffers();
		time += (float)adjust_framerate() / 60;
	}

	//vect();
	//close_copper();
	return 0;
}
