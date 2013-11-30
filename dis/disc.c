/*
 * DIS - C interface, Large memory model
 */

#include <stdio.h>
#include <stdlib.h>
#include "dis.h"
#include "graphics.h"

static int exitflag = 0;	/* 1=exit key pressed */
static int indemoflag = 0;	/* 1=inside the great big ultra cool demo */
static int passmuscode = 0;
//static int forcebreak;

static char msgarea0[64];
static char msgarea1[64];
static char msgarea2[64];
static char msgarea3[64];


static void checkkeys()
{
}

/**
 * entry: -
 *  exit: AX=version
 * descr: -
 */
int dis_version()
{
	indemoflag = 0;
	passmuscode = 0;
	exitflag = 0;

	return 0x0100;
}

/**
 * entry: -
 *  exit: -
 * descr: Waits for border start
 */
int dis_waitb()
{
	return adjust_framerate();		/* number of frames taken            ;TEMP! */
}

#if 0
public _dis_waitb ;int _dis_waitb(void)
_dis_waitb PROC FAR
	mov	bx,1
	int	0fch
	ret
_dis_waitb ENDP
#endif
		
/**
 *
 */
void dis_partstart()
{
	if (dis_version() == 0) {
		fprintf(stderr, "ERROR: DIS not loaded.\n");
		exit(3);
	}
}

/**
 * entry: -
 *  exit: AX=return
 * descr: returns 1 if part should exit.
 */
int dis_exit()
{
	checkkeys();
	return exitflag;
}

/**
 * entry: -
 *  exit: AX=return
 * descr: returns 1 if inside demo (and not testing from dos)
 */
int dis_indemo()
{
	return indemoflag;
}

/**
 * entry: AX=area desired (0..3)
 *  exit: DX:AX=pointer to msgarea
 * descr: returns a pointer to interpart communications area.
 *        There is ONLY 64 bytes of space in the area, DON'T OVERFLOW IT :-)
 */
void *dis_msgarea(int num)
{
	switch (num) {
	case 1:
		return msgarea1;
	case 2:
		return msgarea2;
	case 3:
		return msgarea3;
	default:
		return msgarea0;
	}
}

/**
 * entry: AX=code you want (are waiting for)
 *  exit: AX=current code,BX=row
 */
int dis_muscode(int code)
{
	return 0;
}

#if 0
public _dis_muscode ;int _dis_muscode(void)
_dis_muscode PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	bx,6
	int	0fch
	pop	bp
	ret
_dis_muscode ENDP
#endif

int dis_musplus()
{
	return 0;
}

#if 0
public _dis_musplus ;int _dis_musplus(void)
_dis_musplus PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	bx,6
	int	0fch
	mov	ax,dx
	pop	bp
	ret
_dis_musplus ENDP
#endif

int dis_musrow()
{
	return 0;
}

#if 0
public _dis_musrow ;int _dis_musrow(void)
_dis_musrow PROC FAR
	push	bp
	mov	bp,sp
	mov	bx,6
	int	0fch
	mov	ax,bx
	pop	bp
	ret
_dis_musrow ENDP
#endif

/**
 * entry: AX=number of copper interrupt to capture:
 *                0=after display start (about scan line 25)
 *                1=just before retrace (AVOID USING THIS IF POSSIBLE)
 *                2=in the retrace
 *        DX:CX=far pointer to routine (0:0=remove routine)
 *        The routine pointed to must end in a RETF. It must save any
 *        386 registers it uses (including FS/GS)
 *  exit: -
 * descr: sets the specified copper interrupt to call the specified routine.
 *        IMPORTANT: The part must reset the copper int before it exits!
 */
void dis_setcopper(int routine_number, void (*routine)(void))
{
}

#if 0
public _dis_setcopper ;void _dis_copper(int routine_number,void *routine)
_dis_setcopper PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	cx,[bp+8]
	mov	dx,[bp+10]
	mov	bx,7
	int	0fch
	pop	bp
	ret
_dis_setcopper ENDP
#endif

/**
 *
 */
void dis_setmframe(int frame)
{
}
		
#if 0
public _dis_setmframe ;void _dis_setmframe(int frame)
_dis_setmframe PROC FAR
	push	bp
	mov	bp,sp
	mov	dx,[bp+6]
	mov	ax,1
	mov	bx,9
	int	0fch
	pop	bp
	ret
_dis_setmframe ENDP
#endif
		
/**
 *
 */
int dis_getmframe()
{
	return 0;
}

#if 0
public _dis_getmframe ;void _dis_getmframe(void)
_dis_getmframe PROC FAR
	push	bp
	mov	bp,sp
	xor	ax,ax
	mov	bx,9
	int	0fch
	pop	bp
	ret
_dis_getmframe ENDP
#endif

static const int ordersync1[] = {	// startpart
	0x0000, 0,
	0x0200, 1,
	0x0300, 2,
	0x032f, 3,
	0x042f, 4,
	0x052f, 5,
	0x062f, 6,
	0x072f, 7,
	0x082f, 8,
	0x0900, 9,
	0x0d00, 10,
	0x3d00, 1,
	0x3f00, 2,
	0x4100, 3,
	0x4200, 4
};

/**
 * 
 */
int dis_sync()
{
	// get part from ord, row

	return ordersync1[1];
}

