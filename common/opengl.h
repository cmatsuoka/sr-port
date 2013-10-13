#ifndef SR_OPENGL_H_
#define SR_OPENGL_H_

#include <string.h>

typedef float Matrix[16];

struct program {
	GLuint program;
	GLuint pMatrix_location;
	GLuint uMatrix_location;
	GLuint aPosition_location;
	GLuint uColor_location;
};

extern Matrix identity;

static inline void matrix_identity(Matrix m) {
        memcpy(m, identity, sizeof(identity));
}

static inline void matrix_translate(Matrix m, float x, float y) {
	m[12] = x;
	m[13] = y;
}

static inline void matrix_set(struct program *p, float *m) {
	glUniformMatrix4fv(p->uMatrix_location, 1, 0, m);
}

static inline void set_color(GLfloat *c, struct program *p) {
	glUniform4fv(p->uColor_location, 1, c);
}

int init_opengl(int, int);
void check_error(char *);
void draw_triangle_strip(struct program *, float *, int);
void applyOrtho(float, float, float, float, struct program *);
int create_program(struct program *, GLuint, GLuint);
GLuint compile_vertex_shader(const char *);
GLuint compile_fragment_shader(const char *);

#endif
