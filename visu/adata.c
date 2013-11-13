#include "c.h"

/* projection clip window */
int projclipx[2] = { 0, 319 };		/* (xmin,xmax) */
int projclipy[2] = { 0, 199 };		/* (ymin,ymax) */
int projclipz[2] = { 256, 1000000000 };		/* (zmin,zmax) */

/* projection variables */
int projmulx = 250;
int projmuly = 220;
int projaddx = 100;
int projaddy = 100;
int projaspect = 256;	/* ;aspect ratio (ratio=256*ypitch/xpitch) */

