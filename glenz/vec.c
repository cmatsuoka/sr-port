#include <stdio.h>
#include <string.h>
#include "glenz.h"
#include "vec.h"

int lightshift = 0;
float xadd = 0.0f;
float yadd = 0.0f;
float zadd = 0.0f;
int mtrm[9];

/* For 320x200 */
int projxmul = 256;
int projymul = 213;
int projxadd = 160;
int projyadd = 130;
int projminz = 128;
int projminzshr = 7;
int wminx = 0;
int wminy = 0;
int wmaxx = 319;
int wmaxy = 199;
int framerate10 = 700;


int csetmatrix(int *matrix, int x, int y, int z)
{
	memcpy(mtrm, matrix, 9 * sizeof(int));
	xadd = x;
	yadd = y;
	zadd = z;

	return 0;
}

int crotlist(int *dest, int *src)
{
	int count = *src++;
	int size = *dest;

	*dest += count;
	dest += size * 3 + 1;
	
	while (count--) {
		dest[0] = ((mtrm[0] * src[0] + mtrm[1] * src[1] +
			    mtrm[2] * src[2]) >> 15) + xadd;

		dest[1] = ((mtrm[3] * src[0] + mtrm[4] * src[1] +
			    mtrm[5] * src[2]) >> 15) + yadd;

		dest[2] = ((mtrm[6] * src[0] + mtrm[7] * src[1] +
			    mtrm[8] * src[2]) >> 15) + zadd;

		src += 3;
		dest += 3;
	}

	return 0;
}

int cprojlist(int *dest, int *src)
{
	int count = *src++;
	int size = *dest;

	*dest += count;
	dest += size * 3 + 1;

	while (count--) {
		int x = src[0];
		int y = src[1];
		int z = src[2];
		int bp = 0;
		int xx, yy;
		
		dest[3] = z;
		if (z < projminz) {
			z = projminz;
			bp |= 16;
		}

		/* Store Y */
		yy = y * projymul / z + projyadd;
		if (yy > wmaxy)
			bp |= 8;
		if (yy < wminy)
			bp |= 4;
		dest[1] = yy;

		/* Store X */
		xx = x * projxmul / z + projxadd;
		if (xx > wmaxx)
			bp |= 2;
		if (xx < wminx)
			bp |= 1;
		dest[0] = xx;

		/* Store visibility flags */
		dest[2] = bp;

		src += 3;
		dest += 4;
	}
	
	return 0;
}

int ccliplist(int *points)
{
	int count = *points++;

	while (count--) {
		if (points[1] > 1500)
			points[1] = 1500;
		points += 3;
	}

	return 0;
}


int *edgesoff = 0;
//int cntoff = 0; 

int checkhiddenbx(int *list)
{
	int x1 = list[0];
	int y1 = list[1];
	int x2 = list[2];
	int y2 = list[3];
	int x3 = list[4];
	int y3 = list[5];

	return (x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3);
}

int cpolylist(int *polylist, int *polys, int *edges, int *points3)
{
	return 0;
}

#if 0
_cpolylist PROC FAR ;polylist,polys,edges,points3
	CBEG
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	ax,[bp+14]
	add	ax,4
	mov	cs:edgesoff,ax
	mov	fs,word ptr [bp+16]
	mov	ax,[bp+18]
	add	ax,4
	mov	cs:pointsoff,ax
	mov	gs,word ptr [bp+20]
	mov	bp,-1
@@2:	lodsw
	cmp	ax,0
	je	@@1
	add	di,2
	mov	cx,ax
	movsw
	mov	cs:cntoff,di
@@3:	push	cx
	mov	bx,ds:[si]
	add	si,2
	shl	bx,3
	add	bx,cs:edgesoff
	test	word ptr fs:[bx+4],8000h
	jnz	@@7
	mov	ax,fs:[bx+2]
	cmp	ax,bp
	je	@@5
	mov	ax,fs:[bx]
	call	adddot
	mov	ax,fs:[bx+2]
	call	adddot
@@7:	pop	cx
	loop	@@3
	jmp	@@6
@@5:	mov	ax,fs:[bx+2]
	call	adddot
	mov	ax,fs:[bx]
	call	adddot
	pop	cx
	loop	@@3
@@6:	mov	bx,cs:cntoff
	mov	eax,es:[bx]
	cmp	eax,es:[di-4]
	jne	@@4
	sub	di,4
@@4:	mov	ax,di
	sub	ax,cs:cntoff
	shr	ax,2
	mov	es:[bx-4],ax
	call	checkhiddenbx
	jnc	@@2
	xor	word ptr es:[bx-2],8000h
	jmp	@@2
@@1:	mov	word ptr es:[di],0
	CEND
_cpolylist ENDP
#endif

#if 0
setpalxxx MACRO
	local	l1,l2,l3
	mov	al,cl
	cmp	al,64
	jb	l1
	mov	al,63
l1:	out	dx,al
	mov	al,bl
	cmp	al,64
	jb	l2
	mov	al,63
l2:	out	dx,al
	mov	al,bh
	cmp	al,64
	jb	l3
	mov	al,63
l3:	out	dx,al
	ENDM
#endif

char backpal[16 * 3] = {
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};

int back1[] = { 0, 4, 8 };
int back2[] = { 4, 0, 0 };
int back3[] = { 7, 0, 0 };
int back4[] = { 13, 0, 0 };

int rolcol[256];
int rolused[256];


void demo_glz2(int visible, int *polylist)
{
	if (visible >= 0) {
		int color = polylist[-1] & 0xff;
		polylist[-1] = (color >> 1) & 0x01;
	}
}

void demo_glz(int visible, int *polylist)
{
	int ax, i;
	int color = polylist[-1] & 0xff;
	int al = rolcol[color];
	int bp = 0;
	int r, g, b;

	if (visible < 0) {
		rolcol[color] = 0;
		rolused[al] = 0;
		polylist[-1] = ((color >> 1) & 0x01) << 2;
		return;
	}

	//@@7
	if (lightshift != 9) {
		ax = (visible >> 8) + (visible >> 9);
	} else {
		//@@x9
		ax = visible >> 7;
	}

	//@@x10
	if (ax < 0) {
		ax = 0;
	}

	if (ax > 63) {
		ax = 63;
	}

	//@@s2
	if (al == 0) {
		bp = 2;
		for (i = 0; i < 15; i++) {
			//@@rc3
			if (rolused[bp] == 0)
				break;
			bp += 2;
		}

		//@rc4
		rolcol[color] = bp;
		rolused[bp] = 1;	
	}

	//@@rc1
	polylist[-1] = rolcol[color] << 3;	// wtf

	if (color & 0x02) {
		printf("BLUE: %d (%d)\n", rolcol[color], ax);
		r = 0;
		g = ax / 2;
		b = ax;
	} else {
		printf("GRAY: %d (%d)\n", rolcol[color], ax);
		r = g = b = ax;
	}

	for (i = 0; i < 16; i++) {
		r += backpal[i * 3 + 0];
		g += backpal[i * 3 + 1];
		b += backpal[i * 3 + 2];
		setrgb(polylist[-1] + i, r, g, b);
	}

}

void (*demomode[])(int, int *) = {
	demo_glz,
	demo_glz,
	demo_glz2
};

int ceasypolylist(int *polylist, int *polys, int *points3)
{
	int num, c;
	int *cntoff;
	int i, visible;

	points3++;		// numv

	while ((num = *polys++) != 0) {	// @@2

		*polylist++ = num;	// number of vertices

		c = *polys++;		// color
		*polylist++ = c;

		cntoff = polylist;

		for (i = 0; i < num; i++) {	// @@3
			int v = *polys++;	// vertex index

			// add dot
			*polylist++ = points3[v * 4];
			*polylist++ = points3[v * 4 + 1];
		}

		visible = checkhiddenbx(cntoff);

		// sets colors etc / hidden faces flipped
		demomode[0](visible, cntoff);
	}
	
	*polylist = 0;

	return 0;
}

void cglenzinit()
{
	//newgroup(0, rows);
}

void cglenzdone()
{
	//newgroup(2, rows);
}

void cglenzpolylist(int *polylist)
{
}

#if 0
PUBLIC _cglenzpolylist
_cglenzpolylist PROC FAR
	CBEG
	LOADDS
	movpar	di,0
	movpar	es,1
	mov	ax,1
	call	__newgroup
	CEND
_cglenzpolylist ENDP
#endif

