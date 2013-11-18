;/****************************************************************************
;** MODULE:	adraw.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Object drawing (and polygon calculations & clipping (included))
;**
;****************************************************************************/

/* Rewritten in C and OpenGL by Claudio Matsuoka, Nov 2013 */

#include <stdio.h>
#include "opengl.h"
#include "c.h"
#include "cd.h"

int newlight[] = { 12118,10603,3030 };

int normallight(nlist *normal)
{
	int x = normal->x;
	int y = normal->y;
	int z = normal->z;
	int ax;

	ax = (x * newlight[0] + y * newlight[1] + z * newlight[2]) >> 16;
	ax >>= (28-7-16);
	ax += 128;

	if (ax > 255)
		ax = 255;
	else if (ax < 0)
		ax = 0;

	return ax;
}


/*
;entry: AX=polyflags,ES:DI=normal for which to calculate light
; exit:	AX=colorshade (based 0)
*/
int calclight(int flags, nlist *normal)
{
	int f = flags & F_SHADE32;

	if (f == 0)
		return 0;

	/* lightsource */

	int light = normallight(normal);
	int cx = 6 - (f >> 10);

	light >>= cx;		/* 0400=5, 0800=4, 0C00=3 */

	if (light < 1)
		light = 1;
	else if (light > 30)
		light = 30;

	return light;
}

/*
	;es:di=normal
	;fs:si=vertex
	;ret: carry=1=hidden
*/

int checkculling(nlist *n, fvlist *v)
{
	return (n->x * v->x + n->y * v->y + n->z * v->z >= 0);
}

int poly1[POLYSIZE];
int poly2[POLYSIZE];

/*
;######## _draw_polylist(polylist *l,polydata *d,vlist *v,pvlist *pv,
;                                                      nlist *n,int f) ########
;entry:	0 l=pointer to a polygon list (in polylist format)
;	2 d=pointer to polygon data (to which polylist points using indices)
;	4 v=pointer to rotated 3D vertices (for 3D clipping)
;	6 pv=pointer to projected vertices (for drawing)
;	8 n=pointer to normals (for culling)
;	10 f=object flags
; exit: -
;descr: draw the contents of the polylist.
*/

void draw_polylist(polylist *l,polydata *d,fvlist *v,pvlist *pv, nlist *n,int f)
{
	int i;
	short projvert[32];
	int normals[48];

	if (!(f & F_VISIBLE))
		return;

	l += 2;		/* skip count - sort vertex */

	//@@1
	for ( ; *l; l++) {
		int poly = *l;

		if (poly == 0)		/* end of list */
			break;

		/* si points to polydata/polygon we are now drawing */
		short *si = (short *)&d[poly];

		short sides = *(unsigned char*)si;
		short flags = *((unsigned char*)si + 1);
		flags = (flags << 8) & (f | 0x0f00);

		poly1[POLYFLAGS] = flags;
		poly1[POLYSIDES] = sides;

		short normal = si[2];
		short *point = &si[3];
		short color = *((unsigned char *)si + 2);

		if (color == -1)	/* check cull */
			continue;

		/* for this face */
		nlist *np = &n[normal];
		fvlist *vp = &v[point[0]];

		if (~flags & F_2SIDE) {
			if (checkculling(np, vp))
				continue;
		}

		int near = 0;

		for (i = 0; i < sides; i++) {
			pvlist *pp = &pv[point[i]];

			projvert[i * 2 + 0] = pp->x;
			projvert[i * 2 + 1] = 200 - pp->y;

			if (pp->vf & VF_NEAR)
				near++;
		}

		// Check near clipping
		if (near == sides)
			continue;

		// Workaround: disable shading for windows
		if (color == 208)
			flags &= ~F_GOURAUD;

		if (flags & F_GOURAUD) {
			int light = 255;
			int f = flags & F_SHADE32;

			for (i = 0; i < sides; i++) {
				/* for each vertex */
				fvlist *ff = &v[point[i]];
				nlist *nn = &n[ff->normal];
				
				normals[i * 3 + 0] = nn->x;
				normals[i * 3 + 1] = nn->y;
				normals[i * 3 + 2] = nn->z;
			}

			if (f != 0) {
				int cx = 6 - (f >> 10);
				light >>= cx;
			}

			if (light < 1)
				light = 1;
			else if (light > 30)
				light = 30;

			color += light;

			draw_poly_diffuse(projvert, normals, sides, color);
		} else {
			color += calclight(flags, np);
			draw_poly(projvert, sides, color);
		}
	}

#if 0
	draw_palette();
#endif
}
