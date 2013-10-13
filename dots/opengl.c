#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GL/glu.h>
#include "opengl.h"
#include "dots.h"

GLfloat color[4];
GLfloat shadow_color[4] = { .17, .17, .17, 1.0 };

static int view_width;
static int view_height;
static float radius;

extern float rotsin;
extern float rotcos;
extern int gravity;
extern int gravityd;
extern int gravitybottom;
extern int bpmin;
extern int bpmax;

struct program dot_program;
struct program floor_program;

static GLuint uRadius_location;

#define SQRT3 1.7320508075688772F
#define SQRT3_3 0.5773502691896257F

#define FF 5.0f
float obj[9] = {
	0.0f, FF * SQRT3_3, 0.0f,
	-FF / 2, FF * -SQRT3_3 / 2, 0.0f,
	FF / 2, FF * -SQRT3_3 / 2, 0.0f
};

float shadow_obj[9] = {
	0.0f, FF * SQRT3_3, FF * 5,
	-FF / 2, FF * -SQRT3_3 / 2, 0.0f,
	FF / 2, FF * -SQRT3_3 / 2, 0.0f, 
};

float floor_obj[12] = {
	0.0f, 0.0f, 0.0f,
	320.0f, 0.0f, 0.0f,
	0.0f, 100.0f, 0.0f,
	320.0f, 100.0f, 0.0f
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

static const char dot_shader[] =
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"uniform float uRadius;\n"
"varying vec3 vPosition;\n"
"varying vec4 vCenter;\n"
"\n"
"void main() {\n"
"    float dist = distance(vPosition, vCenter.xyz);\n"
"    if (dist < uRadius) {\n"
"        gl_FragColor = vec4(uColor.xyz, 1.0);\n"
"    } else { gl_FragColor = vec4(0.0); }\n"
"}\n";

static const char floor_shader[] =
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"varying vec3 vPosition;\n"
"\n"
"void main() {\n"
"    float x = -vPosition.y / 2.0;\n"
//"    float y = -x * x + 2.0 * x / 2.2;\n"
"    float y = pow(1.0 - (x - 1.0) * (x - 1.0), 0.5) / 3.3;\n"
"    gl_FragColor = vec4(y, y, y, 1.0);\n"
"}\n";


int init_opengl(int width, int height)
{
	GLuint v, f;

	view_width = 320;
	view_height = 200;

	v = compile_vertex_shader(vertex_shader);
	f = compile_fragment_shader(dot_shader);
	create_program(&dot_program, f, v);

	f = compile_fragment_shader(floor_shader);
	create_program(&floor_program, f, v);

	uRadius_location = glGetUniformLocation(dot_program.program, "uRadius");
	radius = (2.0 / width * FF) / 1.5;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0, .0, .0, 0);

	return 0;
}

void clear_screen()
{
	float m[16];

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(floor_program.program);

	matrix_identity(m);
	matrix_set(&floor_program, m);
	draw_triangle_strip(&floor_program, floor_obj, 4);

	glUseProgram(dot_program.program);
}

void draw_dot(struct dot *dot)
{
	float m[16];
	float bp = ((dot->z * rotcos - dot->x * rotsin) / 0x10000) + 9000;
	float a = (dot->z * rotsin + dot->x * rotcos) / 0x100;

	float x = (a + a / 8) / bp + 160;
	if (x <= 319) {
		float shadow_y = (0x80000 / bp) + 100;
		if (shadow_y <= 199) {

			/* shadow */

			set_color(shadow_color, &dot_program);

			matrix_identity(m);
			matrix_translate(m, x, 200 - shadow_y);

			matrix_set(&dot_program, m);
			draw_triangle_strip(&dot_program, shadow_obj, 3);

			/* ball */

			dot->yadd += gravity;
			float b = dot->y + dot->yadd;
			if (b >= gravitybottom) {
				dot->yadd = (-dot->yadd * gravityd) / 0x10;
				b += dot->yadd;
			}
			dot->y = b;

			float y = (dot->y * 64) / bp + 100;
			if (y <= 199) {

				color[1] = 1.0f - (bp - 3000) / 12000;
				color[2] = color[1] * 1.1f;
				color[0] = color[1] / 3;

				set_color(color, &dot_program);

				matrix_identity(m);
				matrix_translate(m, x, 200 - y);

				matrix_set(&dot_program, m);
				glUniform1fv(uRadius_location, 1, &radius);

				draw_triangle_strip(&dot_program, obj, 3);
			}
		}
	}
}

void projection()
{
	glUseProgram(floor_program.program);
	applyOrtho(0, view_width, 0, view_height, &floor_program);
	glUseProgram(dot_program.program);
	applyOrtho(0, view_width, 0, view_height, &dot_program);
}
