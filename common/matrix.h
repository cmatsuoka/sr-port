#ifndef U2_MATRIX_H_
#define U2_MATRIX_H_

typedef float Matrix[16];

extern Matrix identity;

static inline void matrix_identity(Matrix m) {
        memcpy(m, identity, sizeof(identity));
}

static inline void matrix_translate(Matrix m, float x, float y) {
	m[12] = x;
	m[13] = y;
}

#endif
