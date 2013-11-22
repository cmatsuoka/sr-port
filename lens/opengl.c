#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>
#include "u2gl.h"

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

static const char fragment_shader_texture[] =
"precision mediump float;\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uColor;\n"
"uniform vec2 uPos;\n"
"uniform vec2 uTexPos;\n"
"uniform float uRadius;\n"
"varying vec3 vPosition;\n"
"varying vec2 vTexPosition;\n"
"\n"
"void main() {\n"
"    float d = distance(gl_FragCoord.xy, uPos) / uRadius;\n"
"    if (d < 0.1) d = 0.1;\n"
"    if (d < 1.0) {\n"
"       float new = 0.7 + 0.2 * pow(d*d, 0.69);\n"
"       vec2 p = uTexPos + (vTexPosition - uTexPos) * new;\n"
"       gl_FragColor = texture2D(uTexture, p) + vec4(0.0, 0.0, 0.3, 0.0);\n"
"    } else gl_FragColor = texture2D(uTexture, vTexPosition);\n"
"}\n";


int uPos_location;
int uTexPos_location;
int uRadius_location;

static float tex_coords[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};


static float color[256][4];

#define CC 32

void setrgb(int c, int r, int g, int b)
{
	float alpha = 0.5f;

	color[c][0] = (float)r / CC;
	color[c][1] = (float)g / CC;
	color[c][2] = (float)b / CC;
	color[c][3] = alpha;
}

void getrgb(int c, char *p)
{
	p[0] = color[c][0] * CC;
	p[1] = color[c][1] * CC;
	p[2] = color[c][2] * CC;
}

void draw_bg()
{
	glUseProgram(bg_program.program);
	u2gl_draw_textured_triangle_strip(&bg_program, bg_obj, 4);
}

static void init_texture()
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	u2gl_set_tex_coords(tex_coords);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lenspic.png",
				&width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glGenerateMipmap(GL_TEXTURE_2D);
	u2gl_check_error("init_texture");
}

extern int window_width;
extern int window_height;

void set_radius(float r)
{
	glUseProgram(bg_program.program);
	r = r * window_width / view_width;
	glUniform1fv(uRadius_location, 1, &r);
}

void set_pos(float x, float y)
{
	float vec[2];

	vec[0] = x * window_width / view_width;
	vec[1] = y * window_height / view_height;
	glUniform2fv(uPos_location, 1, vec);

	vec[0] = x / view_width;
	vec[1] = y / view_height;
	glUniform2fv(uTexPos_location, 1, vec);
}

int init_opengl(int width, int height)
{
	Matrix m;
	GLuint v, f;

	view_width = 320;
	view_height = 200;

	v = u2gl_compile_vertex_shader(vertex_shader_texture);
	f = u2gl_compile_fragment_shader(fragment_shader_texture);
	u2gl_create_program(&bg_program, f, v);

	uPos_location = glGetUniformLocation(bg_program.program, "uPos");
	uTexPos_location = glGetUniformLocation(bg_program.program, "uTexPos");
	uRadius_location = glGetUniformLocation(bg_program.program, "uRadius");

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0, .0, .0, 0);

	matrix_identity(m);
	glUseProgram(bg_program.program);
	u2gl_set_matrix(&bg_program, m);
	u2gl_check_error("init_opengl");

	init_texture();
	u2gl_check_error("init_texture");

	return 0;
}

void blend_alpha()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void blend_color()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void clear_screen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void projection()
{
	u2gl_projection(0, view_width, 0, view_height, &bg_program);
}
