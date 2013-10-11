#ifndef DOTS_OPENGL_H_
#define DOTS_OPENGL_H_

#include <GLES2/gl2.h>

int init_graphics(int, int);
int init_opengl(int, int);
void projection(void);
void swap_buffers(void);
void clear_screen(void);

#endif
