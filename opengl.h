#ifndef __OPENGL_H
#define __OPENGL_H

#include <GLES2/gl2.h>

int init_graphics(int, int);
int init_opengl(int, int);
void set_viewport(int, int);
void unset_viewport(void);
void projection(void);
void swap_buffers(void);
void clear_screen(void);
void screen_projection(void);

#endif
