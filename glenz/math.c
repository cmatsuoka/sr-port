#if 0
include mathsin.inc
#endif

/* rotate sins/coss */
int rxsin = 0;
int rxcos = 0;
int rysin = 0;
int rycos = 0;
int rzsin = 0;
int rzcos = 0;

extern int sintable16[];
extern int costable16[];

static void calcmatrix(int *matrix);

int checkdeg(int angle)
{
	while (angle >= 3600)
		angle -= 3600;
	while (angle < 0)
		angle += 3600;

	return angle;
}

#if 0
checkdeg MACRO reg
	local	l0,l1,l2
l0:	cmp	reg,3600
	jb	l1
	jl	l2
	;>3600
	sub	reg,3600
	jmp	l0
l2:	;<0
	add	reg,3600
	jmp	l0
l1:	ENDM
#endif


void cmatrix_yxz(int rotx, int roty, int rotz, int *matrix)
{
	matrix[1] = rotx; 
	matrix[0] = roty;
	matrix[2] = rotz;

	calcmatrix(matrix);
}


static void calcmatrix(int *matrix)
{
	int rotx = matrix[0];
	int roty = matrix[1];
	int rotz = matrix[2];
	int temp;

	/* ROT-X */
	rotx = checkdeg(rotx);
	rxsin = sintable16[rotx];
	rxcos = costable16[rotx];

	/* ROT-Y */
	roty = checkdeg(roty);
	rysin = sintable16[roty];
	rycos = costable16[roty];

	/* ROT-Z */
	rotz = checkdeg(rotz);
	rzsin = sintable16[rotz];
	rzcos = costable16[rotz];

	/*
	 * matrix equations: rY*rX*rZ
	 *  0=Ycos*Zcos-		 2=Xsin*Ysin*Zcos+	 4=-Xcos*Ysin
	 *    Xsin*Ysin*Zsin		   Ycos*Zsin
	 *  6=-Xcos*Zsin		 8=Xcos*Zcos		10=Xsin
	 *  
	 * 12=Xsin*Ycos*Zsin+		14=Ysin*Zsin-		16=Xcos*Ycos
	 *    Ysin*Zcos			   Xsin*Ycos*Zcos\
	 */

	/* 0 & 7 */
	matrix[7] = ((rysin * rzsin) * 2) / 256;
	temp = ((rycos * rzcos) * 2) / 256;
	matrix[0] = temp;
	matrix[7] -= ((temp * rxsin) * 2) / 256;
	temp = ((rxsin * rysin) * 2) / 256;
	matrix[0] -= ((rzsin * temp) * 2) / 256;

	/* 1 */
	matrix[1] = ((rzcos * temp) * 2) / 256;
	temp = ((rycos * rzsin) * 2) / 256;
	matrix[1] += temp;

	/* 6 */
	matrix[6] = ((rxsin * temp) * 2) / 256;
	matrix[6] += ((rysin * rzcos) * 2) / 256;

	/* 3 */
	matrix[3] = -((rxcos * rzsin) * 2) / 256;

	/* 4 */
	matrix[4] = ((rxcos * rzcos) * 2) / 256;

	/* 2 */
	matrix[2] = -((rxcos * rysin) * 2) / 256;

	/* 8 */
	matrix[8] = ((rxcos * rycos) * 2) / 256;

	/* 5 */
	matrix[5] = rxsin;
}

void calcmatrixsep()
{
}

#if 0
calcmatrixsep PROC NEAR ;calc 3 separate matrices
	;ds:[si+0]=rotx
	;ds:[si+2]=roty
	;ds:[si+4]=rotz
	;dest: ds:[di]
	;load values, check 'em and calc sin/cos
	;ROT-X
	mov	bx,ds:[si]
	checkdeg bx
	mov	ds:[si],bx
	shl	bx,1
	mov	ax,cs:sintable16[bx]
	mov	cs:rxsin,ax
	mov	ax,cs:costable16[bx]
	mov	cs:rxcos,ax
	;ROT-Y
	mov	bx,ds:[si+2]
	checkdeg bx
	mov	ds:[si+2],bx
	shl	bx,1
	mov	ax,cs:sintable16[bx]
	mov	cs:rysin,ax
	mov	ax,cs:costable16[bx]
	mov	cs:rycos,ax
	;ROT-Z
	mov	bx,ds:[si+4]
	checkdeg bx
	mov	ds:[si+4],bx
	shl	bx,1
	mov	ax,cs:sintable16[bx]
	mov	cs:rzsin,ax
	mov	ax,cs:costable16[bx]
	mov	cs:rzcos,ax
	mov	bx,0
	mov	cx,32767
	
	;rX
	mov	ds:[di+0],cx
	mov	ds:[di+2],bx
	mov	ds:[di+4],bx
	mov	ds:[di+6],bx
	mov	ax,cs:rxcos
	mov	ds:[di+8],ax
	mov	ax,cs:rxsin
	mov	ds:[di+10],ax
	mov	ds:[di+12],bx
	mov	ax,cs:rxsin
	neg	ax
	mov	ds:[di+14],ax
	mov	ax,cs:rxcos
	mov	ds:[di+16],ax
	add	di,18
	
	;rY
	mov	ax,cs:rycos
	mov	ds:[di+0],ax
	mov	ds:[di+2],bx
	mov	ax,cs:rysin
	neg	ax
	mov	ds:[di+4],ax
	mov	ds:[di+6],bx
	mov	ds:[di+8],cx
	mov	ds:[di+10],bx
	mov	ax,cs:rysin
	mov	ds:[di+12],ax
	mov	ds:[di+14],bx
	mov	ax,cs:rycos
	mov	ds:[di+16],ax
	add	di,18
	
	;rZ
	mov	ax,cs:rzcos
	mov	ds:[di+0],ax
	mov	ax,cs:rzsin
	mov	ds:[di+2],ax
	mov	ds:[di+4],bx
	mov	ax,cs:rzsin
	neg	ax
	mov	ds:[di+6],ax
	mov	ax,cs:rzcos
	mov	ds:[di+8],ax
	mov	ds:[di+10],bx
	mov	ds:[di+12],bx
	mov	ds:[di+14],bx
	mov	ds:[di+16],cx
	add	di,18
	
	ret
calcmatrixsep ENDP
#endif

void calcmatrix0()
{
}

#if 0
calcmatrix0 PROC NEAR ;rX*rY*rZ
	;ds:[si]=rotx
	;ds:[si]=roty
	;ds:[si]=rotz
	;dest: ds:[di]
	;load values, check 'em and calc sin/cos
	;ROT-X
	mov	bx,ds:[si]
	checkdeg bx
	mov	ds:[si],bx
	shl	bx,1
	mov	ax,cs:sintable16[bx]
	mov	cs:rxsin,ax
	mov	ax,cs:costable16[bx]
	mov	cs:rxcos,ax
	;ROT-Y
	mov	bx,ds:[si+2]
	checkdeg bx
	mov	ds:[si+2],bx
	shl	bx,1
	mov	ax,cs:sintable16[bx]
	mov	cs:rysin,ax
	mov	ax,cs:costable16[bx]
	mov	cs:rycos,ax
	;ROT-Z
	mov	bx,ds:[si+4]
	checkdeg bx
	mov	ds:[si+4],bx
	shl	bx,1
	mov	ax,cs:sintable16[bx]
	mov	cs:rzsin,ax
	mov	ax,cs:costable16[bx]
	mov	cs:rzcos,ax
	
	;matrix equations: rX*rY*rZ ;rZ*rY*rX
	; 0=Ycos*Zcos		 2=Ycos*Zsin		 4=-Ysin
	; 6=Xsin*Zcos*Ysin	 8=Xsin*Ysin*Zsin	10=Ycos*Xsin
	;   -Xcos*Zsin		   +Xcos*Zcos
	;12=Xcos*Zcos*Ysin	14=Xcos*Ysin*Zsin	16=Ycos*Xcos
	;   +Xsin*Zsin		   -Xsin*Zcos
	
	;calculate rotation matrix
	;0,2,10,16,4
	mov	bx,cs:rycos
	mov	ax,cs:rzcos
	imul	bx
	shld	dx,ax,1
	mov	ds:[di+0],dx
	;
	mov	ax,cs:rzsin
	imul	bx
	shld	dx,ax,1
	mov	ds:[di+2],dx
	;
	mov	ax,cs:rxsin
	imul	bx
	shld	dx,ax,1
	mov	ds:[di+10],dx
	;
	mov	ax,cs:rxcos
	imul	bx
	shld	dx,ax,1
	mov	ds:[di+16],dx
	;
	mov	ax,cs:rysin
	neg	ax
	mov	ds:[di+4],ax
	;6,8
	mov	ax,cs:rxsin
	imul	cs:rysin
	shld	dx,ax,1
	mov	cx,dx
	;
	mov	ax,cx
	imul	cs:rzcos
	shld	dx,ax,1
	mov	ds:[di+6],dx
	;
	mov	ax,cx
	imul	cs:rzsin
	shld	dx,ax,1
	mov	ds:[di+8],dx
	;
	mov	bx,cs:rxcos
	mov	ax,cs:rzsin
	imul	bx
	shld	dx,ax,1
	sub	ds:[di+6],dx
	;
	mov	ax,cs:rzcos
	imul	bx
	shld	dx,ax,1
	add	ds:[di+8],dx
	;12,14
	mov	ax,cs:rxcos
	imul	cs:rysin
	shld	dx,ax,1
	mov	cx,dx
	;
	mov	ax,cx
	imul	cs:rzcos
	shld	dx,ax,1
	mov	ds:[di+12],dx
	;
	mov	ax,cx
	imul	cs:rzsin
	shld	dx,ax,1
	mov	ds:[di+14],dx
	;
	mov	bx,cs:rxsin
	mov	ax,cs:rzsin
	imul	bx
	shld	dx,ax,1
	add	ds:[di+12],dx
	;
	mov	ax,cs:rzcos
	imul	bx
	shld	dx,ax,1
	sub	ds:[di+14],dx

	ret
calcmatrix0 ENDP
#endif

#if 0
mulmacro MACRO	row,col
	mov	ax,ds:[si+0+row*6]
	imul	word ptr es:[di+0+col*2]
	mov	cx,dx
	mov	bx,ax
	mov	ax,ds:[si+2+row*6]
	imul	word ptr es:[di+6+col*2]
	add	cx,dx
	add	bx,ax
	mov	ax,ds:[si+4+row*6]
	imul	word ptr es:[di+12+col*2]
	add	cx,dx
	add	bx,ax
	shld	cx,bx,1
	ENDM
#endif

void mulmatrices()
{
}

#if 0
mulmatrices PROC NEAR
	;ds:si=matrix1,es:di=matrix2 => ds:si (overwrite)
	mulmacro 0,0
	push	cx
	mulmacro 0,1
	push	cx
	mulmacro 0,2
	push	cx
	
	mulmacro 1,0
	push	cx
	mulmacro 1,1
	push	cx
	mulmacro 1,2
	push	cx
	
	mulmacro 2,0
	push	cx
	mulmacro 2,1
	push	cx
	mulmacro 2,2

	mov	ds:[si+4+12],cx
	pop	word ptr ds:[si+2+12]
	pop	word ptr ds:[si+0+12]
	pop	word ptr ds:[si+4+6]
	pop	word ptr ds:[si+2+6]
	pop	word ptr ds:[si+0+6]
	pop	word ptr ds:[si+4]
	pop	word ptr ds:[si+2]
	pop	word ptr ds:[si+0]
	ret
mulmatrices ENDP
#endif
	
