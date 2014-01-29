#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include "u2gl.h"
#include "opengl.h"

static int view_width;
static int view_height;

static struct u2gl_program bg_program;

float obj[9];

float bg_obj[12] = {
	0.0f, 0.0f, 0.0f,
	320.0f, 0.0f, 0.0f,
	0.0f, 200.0f, 0.0f,
	320.0f, 200.0f, 0.0f
};

static const char vertex_shader_texture[] =
"uniform mat4 pMatrix;\n"
"uniform mat4 uMatrix;\n"
"attribute vec4 aPosition;\n"
"varying vec3 vPosition;\n"
"attribute vec2 aTexPosition;\n"
"varying vec2 vTexPosition;\n"
"\n"
"void main() {\n"
"    mat4 Matrix = pMatrix * uMatrix;\n"
"    vec4 position = Matrix * aPosition;\n"
"    gl_Position = position;\n"
"    vTexPosition = aTexPosition;\n"
"    vPosition = vec3(position);\n"
"}\n";

static char fragment_shader_texture[20000];

int uRes_location;
int uTime_location;

static float tex_coords[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

void draw_bg(float t)
{
	glUniform1f(uTime_location, t);

	//glActiveTexture(GL_TEXTURE0);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 320, 400, GL_RGB,
	//				GL_UNSIGNED_BYTE, image);
	glUseProgram(bg_program.program);
	u2gl_draw_textured_triangle_strip(&bg_program, bg_obj, 4);
}

#if 0
static void init_texture()
{
	GLuint tex[3];

	u2gl_set_tex_coords(tex_coords);

	glGenTextures(1, tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = calloc(3, 320 * 400);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 320, 400, 0, GL_RGB,
				GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	u2gl_check_error("init_texture 1");
}
#endif

extern int window_width;
extern int window_height;

int init_opengl()
{
	GLuint v, f;
	FILE *in;

	view_width = 320;
	view_height = 200;

	in = fopen("plz.glsl", "r");
	if (in != NULL) {
		fread(fragment_shader_texture, 20000, 1, in);
		fclose(in);
	}

	v = u2gl_compile_vertex_shader(vertex_shader_texture);
	f = u2gl_compile_fragment_shader(fragment_shader_texture);
	u2gl_create_program(&bg_program, f, v);
	u2gl_check_error("create program bg");

	uRes_location = glGetUniformLocation(bg_program.program, "iResolution");
	uTime_location = glGetUniformLocation(bg_program.program, "iGlobalTime");
	u2gl_check_error("get uniform locations");

	glUniform3f(uRes_location, (float)window_width, (float)window_height, 0.0f);
	u2gl_check_error("set resolution");

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0, .0, .0, 0);
	u2gl_check_error("init_opengl");

	//init_texture();
	//u2gl_check_error("init_texture");

	u2gl_projection(0, view_width, 0, view_height, &bg_program);

	return 0;
}

void clear_screen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
