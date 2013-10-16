#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GL/glu.h>
#include "u2gl.h"

static int view_width;
static int view_height;

static struct u2gl_program triangle_program;

#define SQRT3 1.7320508075688772F
#define SQRT3_3 0.5773502691896257F

#define FF 5.0f
float obj[9] = {
	0.0f, FF * SQRT3_3, 0.0f,
	-FF / 2, FF * -SQRT3_3 / 2, 0.0f,
	FF / 2, FF * -SQRT3_3 / 2, 0.0f
};

static const char vertex_shader[] =
"uniform mat4 pMatrix;\n"
"uniform mat4 uMatrix;\n"
"attribute vec4 aPosition;\n"
"varying vec3 vPosition;\n"
"\n"
"void main(){\n"
"    mat4 Matrix = pMatrix * uMatrix;\n"
"    vec4 position = Matrix * aPosition;\n"
"    gl_Position = position;\n"
"    vPosition = vec3(position);\n"
"}\n";

static const char triangle_shader[] =
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"varying vec3 vPosition;\n"
"\n"
"void main() {\n"
"    gl_FragColor = vec4(uColor.xyz, 0.5);\n"
"}\n";



static float color[256][3];

void setrgb(int c, int r, int g, int b)
{
    color[c][0] = (float)r / 64;
    color[c][1] = (float)g / 64;
    color[c][2] = (float)b / 64;
}

static void draw_triangle(float *f, int c)
{
	Matrix m;

	u2gl_set_color(color[c], &triangle_program);

	matrix_identity(m);
	//matrix_translate(m, x, 200 - y);

	u2gl_set_matrix(&triangle_program, m);

	obj[0] = *f++;
	obj[1] = *f++;

	obj[3] = *f++;
	obj[4] = *f++;

	obj[6] = *f++;
	obj[7] = *f++;

	u2gl_draw_triangle_strip(&triangle_program, obj, 3);
}

void draw_poly(int *polys, int *points3)
{
	int num, c, i, v;
	float f[6];
	int k = 0;
	int kk = 30;

	points3++;	/* numv */

	while ((num = *polys++) != 0) {
		c = *polys++;

		for (i = 0; i < num; i++) {
			v = *polys++;
			f[i * 2    ] = points3[v * 4];
	 		f[i * 2 + 1] = 200.0f - points3[v * 4 + 1];
		}

		setrgb(k, 20, 20, 20 + kk);
		kk ^= 30;
		draw_triangle(f, k++);
	}
}

int init_opengl(int width, int height)
{
	GLuint v, f;

	view_width = 320;
	view_height = 200;

	v = u2gl_compile_vertex_shader(vertex_shader);
	f = u2gl_compile_fragment_shader(triangle_shader);
	u2gl_create_program(&triangle_program, f, v);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0, .0, .0, 0);

	glUseProgram(triangle_program.program);

	return 0;
}

void clear_screen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void projection()
{
	u2gl_projection(0, view_width, 0, view_height, &triangle_program);
}
