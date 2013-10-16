#include <string.h>
#include "matrix.h"

static Matrix identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};


void matrix_identity(Matrix m)
{
        memcpy(m, identity, sizeof(identity));
}

void matrix_translate(Matrix m, float x, float y)
{
	m[12] = x;
	m[13] = y;
}

