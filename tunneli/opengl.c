#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GL/glu.h>
#include "opengl.h"

static int view_width;
static int view_height;
static float radius;

struct program pixel_program;

static GLuint uRadius_location;

extern float color[256][3];

#define SQRT3 1.7320508075688772F
#define SQRT3_3 0.5773502691896257F

#define FF 3.5f
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
"varying vec4 vCenter;\n"
"\n"
"void main(){\n"
"    mat4 Matrix = pMatrix * uMatrix;\n"
"    vec4 position = Matrix * aPosition;\n"
"    gl_Position = position;\n"
"    vPosition = vec3(position);\n"
"    vCenter = Matrix * vec4(0.0, 0.0, 0.0, 1.0);\n"
"}\n";

static const char pixel_shader[] =
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"uniform float uRadius;\n"
"varying vec3 vPosition;\n"
"varying vec4 vCenter;\n"
"\n"
"void main() {\n"
"    float dist = distance(vPosition, vCenter.xyz);\n"
"    if (dist < uRadius) {\n"
"        gl_FragColor = vec4(uColor.xyz * (uRadius - dist) / uRadius, 1.0);\n"
"    } else { gl_FragColor = vec4(0.0); }\n"
"}\n";


int init_opengl(int width, int height)
{
	GLuint v, f;

	view_width = 320;
	view_height = 200;

	v = compile_vertex_shader(vertex_shader);
	f = compile_fragment_shader(pixel_shader);
	create_program(&pixel_program, f, v);

	uRadius_location = glGetUniformLocation(pixel_program.program, "uRadius");
	radius = (2.0 / width * FF) / 1.5;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0, .0, .0, 0);

	glUseProgram(pixel_program.program);

	return 0;
}

void clear_screen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void draw_pixel(float x, float y, int c)
{
	float m[16];

	set_color(color[c], &pixel_program);

	matrix_identity(m);
	matrix_translate(m, x, 200 - y);

	matrix_set(&pixel_program, m);
	glUniform1fv(uRadius_location, 1, &radius);

	draw_triangle_strip(&pixel_program, obj, 3);
}

void projection()
{
	glUseProgram(pixel_program.program);
	applyOrtho(0, view_width, 0, view_height, &pixel_program);
}
