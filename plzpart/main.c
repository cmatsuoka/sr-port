#include "../dis/dis.h"

extern void plz();
extern void vect();

main()  {
	int a = adjust_framerate();
	dis_partstart();
	init_copper();
	initvect();
	plz();
	vect();
	close_copper();
	return 0;
	}
