#ifndef U2_OPENGL_H_
#define U2_OPENGL_H_

#include <string.h>
#include "matrix.h"

struct u2gl_program {
	GLuint program;
	GLuint pMatrix_location;
	GLuint uMatrix_location;
	GLuint aPosition_location;
	GLuint aNormal_location;
	GLuint aLight_location;
	GLuint uColor_location;
	GLuint uPal_location;
	GLuint aTexPosition_location;
};

static inline void u2gl_set_color(GLfloat *c, struct u2gl_program *p) {
	glUniform4fv(p->uColor_location, 1, c);
}

static inline void u2gl_set_matrix(struct u2gl_program *p, float *m) {
	glUniformMatrix4fv(p->uMatrix_location, 1, 0, m);
}

void	u2gl_set_tex_coords(float *);
void	u2gl_check_error(char *);
void	u2gl_draw_triangle_strip(struct u2gl_program *, float *, int);
void	u2gl_draw_triangle_fan(struct u2gl_program *, float *, int);
void	u2gl_draw_textured_triangle_strip(struct u2gl_program *, float *, int);
void	u2gl_draw_diffuse_triangle_fan(struct u2gl_program *, float *, float *, int);
void	u2gl_set_palette(struct u2gl_program *, float *, int);
void	u2gl_set_light_position(struct u2gl_program *, float *);
void	u2gl_projection(float, float, float, float, struct u2gl_program *);
int	u2gl_create_program(struct u2gl_program *, GLuint, GLuint);
GLuint	u2gl_compile_vertex_shader(const char *);
GLuint	u2gl_compile_fragment_shader(const char *);

#endif
