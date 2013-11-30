#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include "u2gl.h"
#include "dots.h"

GLfloat color[4];
GLfloat shadow_color[4] = { .17, .17, .17, 1.0 };

static int view_width;
static int view_height;

extern float rotsin;
extern float rotcos;
extern int gravity;
extern int gravityd;
extern int gravitybottom;
extern int bpmin;
extern int bpmax;

static struct u2gl_program dot_program;
static struct u2gl_program floor_program;

float colorshift;

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
"varying vec3 vPosition;\n"
"varying vec4 vCenter;\n"
"\n"
"void main() {\n"
"    float dist = distance(vPosition, vCenter.xyz);\n"
"    if (dist < 0.01) {\n"
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
"    float y = pow(1.0 - (x - 1.0) * (x - 1.0), 0.5) / 3.3;\n"
"    if (uColor.x > 1.0) { y = 2.0 - uColor.x; }\n"
"    else if (uColor.x > 0.0) { y = y + (1.0 - y) * uColor.x; }\n"
"    gl_FragColor = vec4(y, y, y, 1.0);\n"
"}\n";


int init_opengl()
{
	GLuint v, f;

	view_width = 320;
	view_height = 200;

	v = u2gl_compile_vertex_shader(vertex_shader);
	f = u2gl_compile_fragment_shader(dot_shader);
	u2gl_create_program(&dot_program, f, v);

	f = u2gl_compile_fragment_shader(floor_shader);
	u2gl_create_program(&floor_program, f, v);

	//radius = 1.0 / 640 * FF * 1.5;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	u2gl_projection(0, view_width, 0, view_height, &floor_program);
	u2gl_projection(0, view_width, 0, view_height, &dot_program);

	return 0;
}

#define SHIFT_TO_WHITE(x) ((x) + (1.0f - (x)) * colorshift)

void shift_color(float *color2, float *color)
{
	if (colorshift > 1.0f) {
		color2[0] = 2.0f - colorshift;
		color2[1] = 2.0f - colorshift;
		color2[2] = 2.0f - colorshift;
	} else if (colorshift > 0.0f) {
		color2[0] = SHIFT_TO_WHITE(color[0]);
		color2[1] = SHIFT_TO_WHITE(color[1]);
		color2[2] = SHIFT_TO_WHITE(color[2]);
	} else {
		color2[0] = color[0];
		color2[1] = color[1];
		color2[2] = color[2];
	}
}

void clear_screen()
{
	Matrix m;
	float color[3] = { 0.0f, 0.0f, 0.0f };
	float color2[3];

	shift_color(color2, color);
	glClearColor(color2[0], color2[1], color2[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(floor_program.program);

	matrix_identity(m);

	color2[0] = colorshift;

	u2gl_set_matrix(&floor_program, m);
	u2gl_set_color(color2, &floor_program);
	u2gl_draw_triangle_strip(&floor_program, floor_obj, 4);

	glUseProgram(dot_program.program);
}

void draw_dot(struct dot *dot)
{
	Matrix m;
	float bp = ((dot->z * rotcos - dot->x * rotsin) / 0x10000) + 9000;
	float a = (dot->z * rotsin + dot->x * rotcos) / 0x100;
	float color2[3];

	float x = (a + a / 8) / bp + 160;
	if (x <= 319) {
		float shadow_y = (0x80000 / bp) + 100;
		if (shadow_y <= 199) {

			/* shadow */

			shift_color(color2, shadow_color);
			u2gl_set_color(color2, &dot_program);

			matrix_identity(m);
			matrix_translate(m, x, 200 - shadow_y);

			u2gl_set_matrix(&dot_program, m);
			u2gl_draw_triangle_strip(&dot_program, shadow_obj, 3);

#if 0
			/* ball */

			dot->yadd += gravity;
			float b = dot->y + dot->yadd;
			if (b >= gravitybottom) {
				dot->yadd = (-dot->yadd * gravityd) / 0x10;
				b += dot->yadd;
			}
			dot->y = b;
#endif

			float y = (dot->y * 64) / bp + 100;
			if (y <= 199) {
				color[1] = 0.75f - (bp - 4000) / 16000;
				color[2] = color[1] * 1.1f;
				color[0] = color[1] / 3;

				shift_color(color2, color);

				u2gl_set_color(color2, &dot_program);

				matrix_identity(m);
				matrix_translate(m, x, 200 - y);

				u2gl_set_matrix(&dot_program, m);
				u2gl_draw_triangle_strip(&dot_program, obj, 3);
			}
		}
	}
}

