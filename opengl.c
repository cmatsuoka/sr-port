#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GL/glu.h>
#include "opengl.h"
#include "dots.h"

GLfloat color[4];
GLfloat shadow_color[4] = { .1, .1, .1, 0 };

static int view_width;
static int view_height;
static int screen_width;
static int screen_height;

extern float rotsin;
extern float rotcos;
extern int gravity;
extern int gravityd;
extern int gravitybottom;
extern int bpmin;
extern int bpmax;

static GLuint pMatrix_location;
static GLuint uMatrix_location;
static GLuint uColor_location;
static GLuint aPosition_location;
static GLuint program;

#define FF 4.0
float obj[9] = {
	0.0f, FF, 0.0f,
	-FF / 2, 0.0f, 0.05f,
	FF / 2, 0.0f, 0.0f
};

float shadow_obj[9] = {
	0.0f, FF / 4, 0.0f,
	-FF / 2, 0.0f, 0.05f,
	FF / 2, 0.0f, 0.0f
};

static const char vertex_shader[] =
"uniform mat4 pMatrix;\n"
"uniform mat4 uMatrix;\n"
"\n"
"attribute vec4 aPosition;\n"
"varying vec3 vPosition;\n"
"\n"
"void main(){\n"
"    vec4 position = pMatrix * uMatrix * aPosition;\n"
"    gl_Position = position;\n"
"    vPosition = vec3(position);"
"}\n";

static const char fragment_shader[] =
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"varying vec3 vPosition;\n"
"\n"
"void main() {\n"
"    gl_FragColor = vec4(uColor);\n"
"}\n";

void check_error(char *t)
{
	int e = glGetError();
	if (e != GL_NO_ERROR) {
		fprintf(stderr, "[%s] %s\n", t, gluErrorString(e));
	}
}

void set_color(GLfloat *c)
{
	glUniform4fv(uColor_location, 1, c);
}

void applyOrtho(float left, float right, float bottom, float top, float near,
		float far)
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

	glUniformMatrix4fv(pMatrix_location, 1, 0, ortho);
}

int init_opengl(int width, int height)
{
	GLuint v, f;
	const char *p;
	char msg[512];

	view_width = 320;
	view_height = 200;
	screen_width = width;
	screen_height = height;

	/* Compile the vertex shader */
	p = vertex_shader;
	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &p, NULL);
	glCompileShader(v);
	glGetShaderInfoLog(v, sizeof msg, NULL, msg);
	printf("vertex shader info: %s\n", msg);

	/* Compile the fragment shader */
	p = fragment_shader;
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &p, NULL);
	glCompileShader(f);
	glGetShaderInfoLog(f, sizeof msg, NULL, msg);
	printf("fragment shader info: %s\n", msg);

	/* Create and link the shader program */
	program = glCreateProgram();
	glAttachShader(program, v);
	glAttachShader(program, f);
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	check_error("ATTACH");

	glLinkProgram(program);
	glGetProgramInfoLog(program, sizeof msg, NULL, msg);
	printf("info: %s\n", msg);

	/* Enable the shaders */
	glUseProgram(program);

	/* Get the locations of the uniforms so we can access them */
	pMatrix_location = glGetUniformLocation(program, "pMatrix");
	uMatrix_location = glGetUniformLocation(program, "uMatrix");
	aPosition_location = glGetAttribLocation(program, "aPosition");

	glEnableVertexAttribArray(aPosition_location);

	//NormalMatrix_location = glGetUniformLocation(program, "NormalMatrix");
	//LightSourcePosition_location = glGetUniformLocation(program, "LightSourcePosition");
	//MaterialColor_location = glGetUniformLocation(program, "MaterialColor");

	uColor_location = glGetUniformLocation(program, "uColor");

	glDisable(GL_DEPTH_TEST);

	glClearColor(.2, .2, .2, 0);
	clear_screen();

	return 0;
}

void clear_screen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void draw_dot(struct dot *dot)
{
	float mi[16], mu[16];

	memset(mi, 0, sizeof(mi));
	mi[0] = mi[5] = mi[10] = mi[15] = 1;

	float bp = ((dot->z * rotcos - dot->x * rotsin) / 0x10000) + 9000;
	float a = (dot->z * rotsin + dot->x * rotcos) / 0x100;

	float x = (a + a / 8) / bp + 160;
	if (x <= 319) {

		float shadow_y = (0x80000 / bp) + 100;
		if (shadow_y <= 199) {

			/* todo: shadow */

			set_color(shadow_color);
			memcpy(mu, mi, sizeof(mi));
			mu[12] = x;
			mu[13] = 200 - shadow_y;

			glUniformMatrix4fv(uMatrix_location, 1, 0, mu);

			glVertexAttribPointer(aPosition_location, 3,
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

				color[0] = 0;
				color[1] = 1.0f - (bp - 3000) / 12000;
				color[2] = color[1];

				set_color(color);

				memcpy(mu, mi, sizeof(mi));
				mu[12] = x;
				mu[13] = 200 - y;

				glUniformMatrix4fv(uMatrix_location, 1, 0, mu);

				glVertexAttribPointer(aPosition_location, 3,
					GL_FLOAT, 0, 3 * sizeof(float), obj);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
				check_error("DRAW");
			}
		}
	}
}

void unset_viewport()
{
	glViewport(0, 0, screen_width, screen_height);
}

void projection()
{
	applyOrtho(0, view_width, 0, view_height, -100, 100);
}
