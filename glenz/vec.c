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


int edgesoff = 0;
int pointsoff = 0;
int cntoff = 0; 

void adddot(int *polylist, int *points3, int x)
{
	if (x == -1)
		return;

	//*polylist = points3[x * 3 + pointsoff];
}

#if 0
adddot PROC NEAR
	cmp	ax,bp
	je	@@3
	mov	bp,ax
	;add dot
	push	bx
	mov	bx,ax
	shl	bx,1
	add	bx,ax
	shl	bx,2
	add	bx,cs:pointsoff
	mov	eax,gs:[bx]
	stosd
	pop	bx
@@3:	ret
adddot ENDP
#endif

void checkhiddenbx(int *src)
{
	(src[0] - src[2]) * (src[1] - src[4 + 1]);

	(src[1] - src[2 + 1]) * (src[0] - src[4]);

	
}

#if 0
checkhiddenbx PROC NEAR
	push	si
	mov	ax,es:[bx]
	mov	si,es:[bx+2]
	sub	ax,es:[bx+4]
	sub	si,es:[bx+8+2]
	imul	si
	push	ax
	push	dx
	mov	ax,es:[bx+2]
	mov	si,es:[bx]
	sub	ax,es:[bx+4+2]
	sub	si,es:[bx+8]
	imul	si
	pop	cx
	pop	si
	xchg	si,ax
	xchg	dx,cx
	sub	ax,si
	sbb	dx,cx
	pop	si
	cmp	dx,0
	jl	@@1
	clc
	ret
@@1:	stc
	ret
checkhiddenbx ENDP
#endif

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

int backpal[16 * 3] = {
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


void demo_norm()
{
}

#if 0
demo_norm PROC NEAR
	ret
demo_norm ENDP
#endif

void demo_glz2(int visible)
{
	if (visible) {
		
	}
}

#if 0
demo_glz2 PROC NEAR
	jc	@@1
	mov	al,es:[bx-2]
	shr	al,1
	and	al,1
	mov	es:[bx-2],al
@@1:	ret
demo_glz2 ENDP
#endif

void demo_glz()
{
	if (lightshift != 9) {
	}
}

#if 0
demo_glz PROC NEAR
	jnc	@@7 ;visible
	mov	ax,es:[bx-2]
	push	bx
	push	ax
	mov	bl,al
	xor	bh,bh
	mov	al,cs:rolcol[bx]
	mov	byte ptr cs:rolcol[bx],0
	mov	bl,al
	mov	cs:rolused[bx],0
	pop	ax
	pop	bx
	shr	al,1
	and	al,1
	shl	al,2
	mov	es:[bx-2],ax
	ret
@@7:	mov	cl,cs:_lightshift
	cmp	cl,9
	je	@@x9
	if (lightshift != 9) {
	}
	shrd	ax,dx,8
	push	ax	
	shrd	ax,dx,1
	pop	dx
	add	ax,dx
	jmp	@@x10
@@x9:	mov	cl,7
	shrd	ax,dx,cl
@@x10:	cmp	ax,0
	jge	@@s1
	mov	ax,0
@@s1:	cmp	ax,63
	jle	@@s2
	mov	ax,63
@@s2:	mov	ah,al
	;
	push	bx
	movzx	bx,es:[bx-2]
	mov	al,cs:rolcol[bx]
	cmp	al,0
	jne	@@rc1
	mov	cx,15
	mov	bp,2
@@rc3:	cmp	cs:rolused[bp],0
	je	@@rc4
	add	bp,2
	loop	@@rc3
@@rc4:	mov	ax,bp
@@rc2:	mov	cs:rolcol[bx],al
	mov	cs:rolused[bp],1
@@rc1:	pop	bx
	mov	bp,es:[bx-2]
	shl	al,3
	mov	es:[bx-2],al
	
	mov	dx,3c8h
	out	dx,al
	inc	dx

	test	bp,2
	jz	@@b1	
	mov	bh,ah ;B
	shr	ah,1
	mov	bl,ah ;G
	jmp	@@b2
@@b1:	mov	cl,ah ;R
	mov	bl,ah ;G
	mov	bh,ah ;B
@@b2:	push	si
	push	di
	mov	si,cx
	mov	di,bx
	
	zzz=0
	REPT	16
	mov	al,cs:_backpal[zzz*3+0]
	shr	al,2
	add	cl,al
	mov	al,cs:_backpal[zzz*3+1]
	shr	al,2
	add	bl,al
	mov	al,cs:_backpal[zzz*3+2]
	shr	al,2
	add	bh,al
	setpalxxx
	mov	bx,di
	mov	cx,si
	zzz=zzz+1
	ENDM
	
	pop	di
	pop	si
@@xx:	ret
demo_glz ENDP
#endif

void (*demomode[])() = {
	demo_glz,
	demo_glz,
	demo_glz2
};

int ceasypolylist(int *polylist, int *polys, int *points3)
{
	int *pointsoff = points3 + 1;
	int *cntoff;
	int i;

	while (*polys != 0) {	// @@2
			
		*polylist++ = *polys++;
		cntoff = polylist;

		for (i = 0; i < *polys; i++) {	// @@3
			adddot(polylist++, points3, *polys++);
		}

		if (*cntoff == polylist[-2]) {
			polylist -= 2;
		}

		*(cntoff - 2) = (polylist - cntoff) / 4;
		checkhiddenbx(*(cntoff - 2));

		demomode[0]();  // sets colors etc / hidden faces flipped
	}
	

	return 0;
}

#if 0
PUBLIC _ceasypolylist
_ceasypolylist PROC FAR ;polylist,polys,points3
	CBEG
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]	; es:di = polylist
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]	; ds:si = polys
	mov	ax,[bp+14]
	add	ax,4
	mov	cs:pointsoff,ax
	mov	gs,word ptr [bp+16]	; gs:ax = points3
	mov	bp,-1

@@2:	lodsw
	cmp	ax,0
	je	@@1
	add	di,2
	mov	cx,ax
	movsw
	mov	cs:cntoff,di

@@3:	push	cx
	mov	ax,ds:[si]
	add	si,2
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
	call	cs:_demomode ;sets colors etc / hidden faces flipped
	jmp	@@2

@@1:	mov	word ptr es:[di],0
	CEND
_ceasypolylist ENDP
#endif

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

