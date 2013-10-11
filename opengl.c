#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GL/glu.h>
#include "opengl.h"
#include "dots.h"

GLfloat color[4];
GLfloat shadow_color[4] = { .2, .2, .2, 1.0 };

static int view_width;
static int view_height;
static int screen_width;
static int screen_height;

static float radius;
static float mi[16];

extern float rotsin;
extern float rotcos;
extern int gravity;
extern int gravityd;
extern int gravitybottom;
extern int bpmin;
extern int bpmax;

struct program {
	GLuint program;
	GLuint pMatrix_location;
	GLuint uMatrix_location;
	GLuint aPosition_location;
	GLuint uColor_location;
};

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
	0.0f, FF * SQRT3_3, FF / 2,
	-FF / 2, FF * -SQRT3_3 / 2, 0.0f,
	FF / 2, FF * -SQRT3_3 / 2, 0.0f
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
"    float x = 0.05 - vPosition.y / 3.6;\n"
"    gl_FragColor = vec4(x, x, x, 1.0);\n"
"}\n";


void check_error(char *t)
{
	int e = glGetError();
	if (e != GL_NO_ERROR) {
		fprintf(stderr, "[%s] %s\n", t, gluErrorString(e));
	}
}

void set_color(GLfloat *c, struct program *p)
{
	glUniform4fv(p->uColor_location, 1, c);
}

void applyOrtho(float left, float right, float bottom, float top, float near,
		float far, struct program *p)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = -2.0f / (far - near);

	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(far + near) / (far - near);

	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		tx, ty, tz, 1
	};

	glUseProgram(p->program);
	glUniformMatrix4fv(p->pMatrix_location, 1, 0, ortho);
}

int create_program(struct program *p, GLuint v, GLuint f)
{
	char msg[512];

	p->program = glCreateProgram();
	glAttachShader(p->program, v);
	glAttachShader(p->program, f);
	glBindAttribLocation(p->program, 0, "position");
	check_error("ATTACH");

	glLinkProgram(p->program);
	glGetProgramInfoLog(p->program, sizeof msg, NULL, msg);
	printf("info: %s\n", msg);

	p->pMatrix_location = glGetUniformLocation(p->program, "pMatrix");
	p->uMatrix_location = glGetUniformLocation(p->program, "uMatrix");
	p->aPosition_location = glGetAttribLocation(p->program, "aPosition");
	p->uColor_location = glGetUniformLocation(p->program, "uColor");

	return 0;
}

GLuint compile_vertex_shader(const char *p)
{
	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	char msg[512];

	glShaderSource(v, 1, &p, NULL);
	glCompileShader(v);
	glGetShaderInfoLog(v, sizeof msg, NULL, msg);
	printf("vertex shader info: %s\n", msg);

	return v;
}

GLuint compile_fragment_shader(const char *p)
{
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
	char msg[512];

	glShaderSource(f, 1, &p, NULL);
	glCompileShader(f);
	glGetShaderInfoLog(f, sizeof msg, NULL, msg);
	printf("fragment shader info: %s\n", msg);

	return f;
}

int init_opengl(int width, int height)
{
	GLuint v, f;

	memset(mi, 0, sizeof(mi));
	mi[0] = mi[5] = mi[10] = mi[15] = 1;

	view_width = 320;
	view_height = 200;
	screen_width = width;
	screen_height = height;

	v = compile_vertex_shader(vertex_shader);
	f = compile_fragment_shader(dot_shader);
	create_program(&dot_program, f, v);

	f = compile_fragment_shader(floor_shader);
	create_program(&floor_program, f, v);

	/* Get the locations of the uniforms so we can access them */
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
	float mu[16];

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(floor_program.program);

	glEnableVertexAttribArray(floor_program.aPosition_location);

	memcpy(mu, mi, sizeof(mi));
	glUniformMatrix4fv(floor_program.uMatrix_location, 1, 0, mu);
	glVertexAttribPointer(floor_program.aPosition_location, 3,
			GL_FLOAT, 0, 3 * sizeof(float), floor_obj);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(floor_program.aPosition_location);

	glUseProgram(dot_program.program);
}

void draw_dot(struct dot *dot)
{
	float mu[16];

	float bp = ((dot->z * rotcos - dot->x * rotsin) / 0x10000) + 9000;
	float a = (dot->z * rotsin + dot->x * rotcos) / 0x100;

	glEnableVertexAttribArray(dot_program.aPosition_location);

	float x = (a + a / 8) / bp + 160;
	if (x <= 319) {

		float shadow_y = (0x80000 / bp) + 100;
		if (shadow_y <= 199) {

			/* shadow */

			set_color(shadow_color, &dot_program);
			memcpy(mu, mi, sizeof(mi));
			mu[12] = x;
			mu[13] = 200 - shadow_y;

			glUniformMatrix4fv(dot_program.uMatrix_location, 1, 0, mu);
			glVertexAttribPointer(dot_program.aPosition_location, 3,
				GL_FLOAT, 0, 3 * sizeof(float), shadow_obj);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

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

				memcpy(mu, mi, sizeof(mi));
				mu[12] = x;
				mu[13] = 200 - y;

				glUniformMatrix4fv(dot_program.uMatrix_location, 1, 0, mu);
				glUniform1fv(uRadius_location, 1, &radius);

				glVertexAttribPointer(dot_program.aPosition_location, 3,
					GL_FLOAT, 0, 3 * sizeof(float), obj);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
				check_error("DRAW");
			}
		}
	}

	glDisableVertexAttribArray(dot_program.aPosition_location);
}

void projection()
{
	applyOrtho(0, view_width, 0, view_height, -100, 100, &floor_program);
	applyOrtho(0, view_width, 0, view_height, -100, 100, &dot_program);
}
