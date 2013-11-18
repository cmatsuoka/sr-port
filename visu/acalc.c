;/****************************************************************************
;** MODULE:	acalc.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Calculations
;**
;****************************************************************************/

/* Rewritten in C and OpenGL by Claudio Matsuoka, Nov 2013 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "c.h"
#include "cd.h"

/*
;entry:	fs:si=matrix1, es:di=matrix2
; exit: es:di=matrix1*matrix2 (matrix 2 overwritten)
*/

void mulmatrices2(float *m1, float *m2)
{
	float m[9];
	int i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			m[i * 3 + j] = (m1[i * 3 + 0] * m2[0 + j] +
					m1[i * 3 + 1] * m2[3 + j] +
					m1[i * 3 + 2] * m2[6 + j]) / 16384;
		}
	}

	memcpy(m2, m, 9 * sizeof(float));
}

/*
	;ds:si=rmatrix
	;fs:bp=source[]: x,y,z (long)
	;es:di=destination[]: x,y,z (long)
	;destination and source can be same
*/

void rotatesingle(rmatrix *matrix, float *d)
{
	float *m = matrix->m;
	float v[3];
	int i;

	for (i = 0; i < 3; i++) {
		v[i] = (m[i * 3 + 0] * d[0] +
			m[i * 3 + 1] * d[1] +
			m[i * 3 + 2] * d[2]) / 16384;
	}

	memcpy(d, v, 3 * sizeof(float));
}

/*
;######## _calc_applyrmatrix(rmatrix *dest, rmatrix *apply) ########
;entry: dest=destination matrix (matrix modified)
;	apply=apply matrix (modifying matrix)
; exit:	(data written to dest matrix)
;descr: The apply matrix is the camera matrix, the dest contains
;	the objects own rotation/position, which is modified
;	according to the camera.
*/

void calc_applyrmatrix(rmatrix *dest, rmatrix *apply)
{
	mulmatrices2(apply->m, dest->m);
	/* dest now has the new rotation matrix, next rotate position */
	rotatesingle(apply, &dest->x);
	/* translate */
	dest->m[9]  += apply->m[9];
	dest->m[10] += apply->m[10];
	dest->m[11] += apply->m[11];
}

/*
;######## _calc_rotate(int count,vlist *dest,vlist *source,rmatrix *matrix) ########
;entry:	count=number of vertices to rotate/move
;	dest=destination 3D list
;	source=source 3D list
;	matrix=rmatrix containing rotation / moving
; exit: -
;descr: Rotates (and moves) the given list
*/

void calc_rotate(int count,fvlist *dest,vlist *source,rmatrix *matrix)
{
	int i;
	float *m = matrix->m;

	for (i = 0; i < count; i++) {
		dest->x = (1.0f * (m[0] * source->x + m[1] * source->y +
					m[2] * source->z) / 16384) + matrix->x;

		dest->y = (1.0f * (m[3] * source->x + m[4] * source->y +
					m[5] * source->z) / 16384) + matrix->y;

		dest->z = (1.0f * (m[6] * source->x + m[7] * source->y +
					m[8] * source->z) / 16384) + matrix->z;

		dest->normal = source->normal;

		source++;
		dest++;
	}
}

/*
;######## _calc_singlez(int vertex,vlist *vertexlist,rmatrix *matrix) ########
;entry:	vertex=number of vertex to process
;	vertexlist=list from which to pick the vertex
; exit: -
;descr: Rotates the single vertex and returns the resulting Z coordinate.
*/

int calc_singlez(int vertex,vlist *vertexlist,rmatrix *matrix)
{
	vlist *v = &vertexlist[vertex];
	float *m = matrix->m;

	return ((m[6] * v->x + m[7] * v->y + m[8] * v->z) / 16384) + matrix->z;
}

/*
;######## _calc_nrotate(int count,nlist *dest,nlist *source,rmatrix *matrix) ########
;entry:	count=number of normals to rotate
;	dest=destination 3Dnormal list
;	source=source 3Dnormal list
;	matrix=rmatrix containing rotation (moving part of rmatrix not used)
; exit: -
;descr: Rotates the given normal list
*/

void calc_nrotate(int count,nlist *dest,nlist *source,rmatrix *matrix)
{
	int i;
	float *m = matrix->m;

	for (i = 0; i < count; i++) {
		dest->x = (m[0] * source->x + m[1] * source->y +
					m[2] * source->z) / 16384;

		dest->y = (m[3] * source->x + m[4] * source->y +
					m[5] * source->z) / 16384;

		dest->z = (m[6] * source->x + m[7] * source->y +
					m[8] * source->z) / 16384;

		source++;
		dest++;
	}
}

/*
;######## _calc_project(int count,pvlist *dest,vlist *source) ########
;entry:	count=number of vertices to project
;	dest=destination projected list
;	source=source 3D list
;	(_proj* variables in data segment define the projection)
; exit: logical and of visibility flags for all vertices (!=0 == object invis.)
;descr: Projects the given list = does perspective transformation
*/

int calc_project(int count,pvlist *dest,fvlist *source)
{
	int i;
	int ret = 0xffff;

	for (i = 0; i < count; i++) {
		int vf = 0;
		float x = source->x;
		float y = source->y;
		float z = source->z;

		if (z < projclipz[CLIPMIN]) {
			vf |= VF_NEAR;
			z = projclipz[CLIPMIN];
		} else if (z > projclipz[CLIPMAX]) {
			vf |= VF_FAR;
		}

		y = y * projmuly / z + projaddy;
		if (y > projclipy[CLIPMAX])
			vf |= VF_DOWN;
		if (y < projclipy[CLIPMIN])
			vf |= VF_UP;
		dest->y = y;

		x = x * projmulx / z + projaddx;
		if (x > projclipx[CLIPMAX])
			vf |= VF_RIGHT;
		if (x < projclipx[CLIPMIN])
			vf |= VF_LEFT;
		dest->x = x;

		dest->vf = vf;
		ret &= vf;

		source++;
		dest++;
	}

	return ret;
}
