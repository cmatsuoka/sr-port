#ifndef __OPENGL_H
#define __OPENGL_H

#include <GL/gl.h>

#define set_color(x) glColor4ubv(x)

extern GLubyte red[];
extern GLubyte blue[];
extern GLubyte green[];
extern GLubyte white[];
extern GLubyte black[];
extern GLubyte yellow[];
extern GLubyte orange[];

int init_graphics(int, int);
int init_opengl(int, int);
void set_viewport(int, int);
void unset_viewport(void);
void projection(void);
void swap_buffers(void);
void clear_screen(void);
void view_projection(void);
void screen_projection(void);

#endif
