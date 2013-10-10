
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "opengl.h"
#include "dots.h"

GLubyte color[4];

static int view_width;
static int view_height;
static int screen_width;
static int screen_height;

extern int rotsin;
extern int rotcos;
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

void draw_line2d(float x0, float y0, float x1, float y1)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glEnd();
}

void draw_dot(struct dot *dot)
{
	float f = 4.0;

	glPushMatrix();

	int bp = ((dot->z * rotcos - dot->x * rotsin) >> 16) + 9000;
	int a = (dot->z * rotsin + dot->x * rotcos) >> 8; 

	int x = (a + a / 8) / bp + 160;
	if (x <= 319) {
	
		int shadow_y = (0x80000 / bp) + 100;
		if (shadow_y <= 199) {

			/* todo: shadow */


			/* ball */
	
			dot->yadd += gravity;
			int b = dot->y + dot->yadd;
			if (b >= gravitybottom) {
				dot->yadd = (-dot->yadd * gravityd) >> 4;
				b += dot->yadd;
			}
			dot->y = b;
	
			int y = (dot->y * 64) / bp + 100;
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

void screen_projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screen_width, screen_height, 0, 0, 1);
}

