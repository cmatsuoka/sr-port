
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "opengl.h"
#include "dots.h"

GLubyte color[4];
GLubyte shadow_color[4] = { .1, .1, .1, 0 };

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


#define set_color(x) glColor4ubv(x)

int init_opengl(int width, int height)
{
	view_width = 320;
	view_height = 200;
	screen_width = width;
	screen_height = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, view_width, view_height, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375, 0.375, 0); /* trick for exact pixelization */

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
	float f = 4.0;

	glPushMatrix();

	float bp = ((dot->z * rotcos - dot->x * rotsin) / 0x10000) + 9000;
	float a = (dot->z * rotsin + dot->x * rotcos) / 0x100; 

	float x = (a + a / 8) / bp + 160;
	if (x <= 319) {
	
		float shadow_y = (0x80000 / bp) + 100;
		if (shadow_y <= 199) {

			/* todo: shadow */

			glPushMatrix();
			set_color(shadow_color);
			glTranslatef(x, 200 - shadow_y, 0.0f);
	
			glBegin(GL_TRIANGLES); // Draw a triangle
			glVertex3f(0.0f, f / 4, 0.0f);
			glVertex3f(-f/2, 0.0f, 0.05f);
			glVertex3f(f/2, 0.0f, 0.0f);
			glEnd();
			glPopMatrix();

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
				color[1] = 255 - 255 * (bp - 3000) / 12000;
				color[2] = color[1];

				set_color(color);
				glTranslatef(x, 200 - y, 0.0f);
		
				glBegin(GL_TRIANGLES); // Draw a triangle
				glVertex3f(0.0f, f, 0.0f);
				glVertex3f(-f/2, 0.0f, 0.05f);
				glVertex3f(f/2, 0.0f, 0.0f);
				glEnd();
			}
		}
	}

	//set_color(green);
	//draw_line(leaf->pos, leaf->vel);
	glPopMatrix();
}

void unset_viewport()
{
	glViewport(0, 0, screen_width, screen_height);
}

void view_projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, view_width, view_height, 0, 0, 1);
}


void projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 320, 0, 200, -100, 100);
}

