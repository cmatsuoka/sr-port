#include <stdio.h>
#include "../dis/dis.h"
#include "graphics.h"
#include "opengl.h"
#include "common.h"

extern void plz();
extern void vect();

int main(int argc, char **argv)  {
	int a = adjust_framerate();
	dis_partstart();

	if (init_graphics("Plzpart", argc, argv) < 0) {
		fprintf(stderr, "Can't init graphics\n");
		return -1;
	};

	init_opengl();
	init_copper();
	initvect();
	plz();
	vect();
	close_copper();
	return 0;
	}
