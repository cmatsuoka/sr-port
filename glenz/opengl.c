#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GLES2/gl2.h>
#include <SOIL/SOIL.h>
#include "u2gl.h"

static int view_width;
static int view_height;

static struct u2gl_program triangle_program;
static struct u2gl_program fc_program;

float obj[9];

float fc_obj[12] = {
	0.0f, 0.0f, 0.0f,
	320.0f, 0.0f, 0.0f,
	0.0f, 50.0f, 0.0f,
	320.0f, 50.0f, 0.0f
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

static const char fragment_shader[] =
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"varying vec3 vPosition;\n"
"\n"
"void main() {\n"
"    gl_FragColor = uColor;\n"
"}\n";

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
"varying vec3 vPosition;\n"
"varying vec2 vTexPosition;\n"
"\n"
"void main() {\n"
"    gl_FragColor = texture2D(uTexture, vTexPosition).rgba * uColor.x;\n"
"}\n";



static float tex_coords[] = {
	0.0f, 0.5f,
	1.0f, 0.5f,
	0.0f, 0.0f,
	1.0f, 0.0f
};


static float color[256][4];

#define CC 32

void setrgb(int c, int r, int g, int b)
{
	float alpha = 0.5f;

#if 0
	if (c == 232 || c == 240) {
		alpha = 0.75f;
	}
#endif
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

void draw_fc()
{
	glUseProgram(fc_program.program);
	u2gl_set_color(color[15], &fc_program);
	u2gl_draw_textured_triangle_strip(&fc_program, fc_obj, 4);
}

static void draw_triangle(float *f, int c)
{
	u2gl_set_color(color[c], &triangle_program);

	obj[0] = *f++;
	obj[1] = *f++;

	obj[3] = *f++;
	obj[4] = *f++;

	obj[6] = *f++;
	obj[7] = *f++;

	u2gl_draw_triangle_strip(&triangle_program, obj, 3);
}

void draw_palette()
{
	int i;

	glUseProgram(triangle_program.program);
	for (i = 0; i < 256; i++) {
		u2gl_set_color(color[i], &triangle_program);
	
		obj[0] = 20 + (i - 64 * (i / 64)) * 4;
		obj[1] = 45 -10 * (i / 64) ;
		obj[3] = obj[0];
		obj[4] = obj[1] - 10;
		obj[6] = obj[0] + 4;
		obj[7] = obj[1] - 10;
	
		u2gl_draw_triangle_strip(&triangle_program, obj, 3);
	}
}

void draw_poly(int *polylist)
{
	int num_vertices, c, i;
	float f[6];
	int *p = polylist;

	glUseProgram(triangle_program.program);
	while ((num_vertices = *polylist++) != 0) {
		c = *polylist++ & 0xff;

		for (i = 0; i < num_vertices; i++) {
			f[i * 2 + 0] = *polylist++;
	 		f[i * 2 + 1] = 200.0f - *polylist++;
		}
		if (c == 232 || c == 240 || c == 0 || c == 4)
			draw_triangle(f, c);
	}

	polylist = p;
	while ((num_vertices = *polylist++) != 0) {
		c = *polylist++ & 0xff;
		for (i = 0; i < num_vertices; i++) {
			f[i * 2 + 0] = *polylist++;
	 		f[i * 2 + 1] = 200.0f - *polylist++;
		}
		if (c != 232 && c != 240 && c != 0 && c != 4)
			draw_triangle(f, c);
	}

#if 0
	draw_palette();
#endif
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
	unsigned char* image = SOIL_load_image("fc.png",
				&width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glGenerateMipmap(GL_TEXTURE_2D);
	u2gl_check_error("init_texture");
}

int init_opengl()
{
	Matrix m;
	GLuint v, f;

	view_width = 320;
	view_height = 200;

	v = u2gl_compile_vertex_shader(vertex_shader);
	f = u2gl_compile_fragment_shader(fragment_shader);
	u2gl_create_program(&triangle_program, f, v);

	v = u2gl_compile_vertex_shader(vertex_shader_texture);
	f = u2gl_compile_fragment_shader(fragment_shader_texture);
	u2gl_create_program(&fc_program, f, v);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0, .0, .0, 0);

	glUseProgram(triangle_program.program);
	matrix_identity(m);
	u2gl_set_matrix(&triangle_program, m);

	glUseProgram(fc_program.program);
	u2gl_set_matrix(&fc_program, m);

	u2gl_check_error("init_opengl");

	init_texture();

	u2gl_projection(0, view_width, 0, view_height, &triangle_program);
	u2gl_projection(0, view_width, 0, view_height, &fc_program);

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
