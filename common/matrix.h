#ifndef U2_MATRIX_H_
#define U2_MATRIX_H_

typedef float Matrix[16];

void matrix_identity(Matrix);
void matrix_translate(Matrix, float, float);

#endif
