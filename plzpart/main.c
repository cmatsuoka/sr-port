#include "../dis/dis.h"
#include "common.h"

extern void plz();
extern void vect();

int main()  {
	int a = adjust_framerate();
	dis_partstart();
	init_copper();
	initvect();
	plz();
	vect();
	close_copper();
	return 0;
	}
