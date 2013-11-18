;/****************************************************************************
;** MODULE:	avid.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Video (drawing, init...)
;**
;****************************************************************************/

/* Rewritten in C and OpenGL by Claudio Matsuoka, Nov 2013 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "graphics.h"
#include "opengl.h"
#include "c.h"
#include "cd.h"

extern int avistan[];

int framecounter = 0;

/*
;entry: -
; exit: -
;descr: does nothing
emptyroutine PROC NEAR
	ret
emptyroutine ENDP
*/
void emptyroutine()
{
}

/*
;entry: -
; exit: -
;descr: executed before using graphics routines at entries.
;	loads es => vram, and sets some VGA registers the
;	way the gfx routines expect them to be.
*/
void vidstart()
{
}

/*
;######## _vid_cameraangle(angle a) ########
;entry:	(see above)
; exit: -
;descr: sets the vision angle (a=0..65535)
*/

void vid_cameraangle(angle a)
{
	int ax, bx = a >> 1;		/* divide by 2 for half of angle */

	ax = projclipx[CLIPMAX] - projaddx;
					/* ax=width of half of screen */

	if (bx < 8 * 64)		/* about 3 degrees minimum */
		bx = 8 * 64;

	if (bx >= 16384)
		bx = 16383;		/* 90 degrees maximum */

	bx >>= 6;			/* bx= index (0..255) */

	projmulx = (ax * avistan[bx]) >> 8;
	projmuly = (projmulx * projaspect) >> 8;
}

/*
;######## _vid_window(long x1,x2,y1,y2,z1,z2) ########
;entry:	(see above)
; exit: -
;descr: sets the video window (for clipping) and sets xadd/yadd to the
;	center of the window
*/

void vid_window(int x1,int x2,int y1,int y2,int z1,int z2)
{
	projclipx[CLIPMIN] = x1;
	projclipx[CLIPMAX] = x2;
	projaddx = (x1 + x2) >> 1;

	projclipy[CLIPMIN] = y1;
	projclipy[CLIPMAX] = y2;
	projaddy = (y1 + y2) >> 1;

	projclipz[CLIPMIN] = z1;
	projclipz[CLIPMAX] = z2;

	set_window(x1, y1, (x2 - x1) + 1, (y2 - y1) + 1);
}

/*
;######## _vid_init(int mode) ########
;entry:	mode (see below)
; exit: -
;descr: initializes screen to graphics and sets up required variables
;	NOTE: the screen / palette is left black.
;modes:	1=320x200x256 (tweak)
*/

void vid_init(int mode)
{
	if (init_graphics("Visu", window_width, window_height) < 0) {
		fprintf(stderr, "Can't init graphics\n");
		return;
	};

	init_opengl(window_width, window_height);
	projection();
	set_fps(35);
}
	
/*
;######## _vid_deinit() ########
;entry:	-
; exit: -
;descr: resets screen to text mode
*/

void vid_deinit()
{
	/* do nothing */
}

/*
;######## _vid_setpal(char far *pal) ########
;entry:	pal=palette (768 bytes VGA RGB)
; exit: -
;descr: sets screen palette 
*/

void vid_setpal(char *pal)
{
	int i;

	for (i = 0; i < 256; i++) {
		int p = i * 3;
		setrgb(i, pal[p], pal[p + 1], pal[p + 2]);
	}
}

/*
;######## _vid_clear(void) ########
;entry: -
; exit: -
;descr: Clears the current screen to black (color 0)
*/
void vid_clear()
{
	clear_screen();
}

/*
;######## _vid_clear255(void) ########
;entry: -
; exit: -
;descr: Clears the current screen to black (color 0)
*/
void vid_clear255()
{
	clear_screen();
}

/*
;######## _vid_clearbg(char *bg) ########
;entry: -
; exit: -
;descr: Copies the bg to screen
*/

//FIXME: add background image
void vid_clearbg(char *bg)
{
	clear_screen();
}

/*
;######## _vid_setswitch(int,int) ########
;entry: -
; exit: -
;descr: Forces a separate write/display switch
_vid_setswitch PROC FAR
	CBEG
	;call	vidstart
	movpar	ax,0
	cmp	ax,-1
	je	@@1
	shl	ax,14-4
	add	ax,0a000h
	mov	ds:_vramseg,ax
@@1:	movpar	ax,1
	cmp	ax,-1
	je	@@2
	shl	ax,14
	mov	dx,3d4h
	mov	al,0ch
	out	dx,ax
@@2:	CEND
_vid_setswitch ENDP
*/

void vid_setswitch(int a,int b)
{
}

