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
	matrix[7] = (rysin * rzsin) >> 15;
	temp = (rycos * rzcos) >> 15;
	matrix[0] = temp;
	matrix[7] -= (temp * rxsin) >> 15;
	temp = (rxsin * rysin) >> 15;
	matrix[0] -= (rzsin * temp) >> 15;

	/* 1 */
	matrix[1] = (rzcos * temp) >> 15;
	temp = (rycos * rzsin) >> 15;
	matrix[1] += temp;

	/* 6 */
	matrix[6] = (rxsin * temp) >> 15;
	matrix[6] += (rysin * rzcos) >> 15;

	/* 3 */
	matrix[3] = -(rxcos * rzsin) >> 15;

	/* 4 */
	matrix[4] = (rxcos * rzcos) >> 15;

	/* 2 */
	matrix[2] = -(rxcos * rysin) >> 15;

	/* 8 */
	matrix[8] = (rxcos * rycos) >> 15;

	/* 5 */
	matrix[5] = rxsin;
}
