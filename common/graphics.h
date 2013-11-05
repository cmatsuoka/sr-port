#ifndef U2_GRAPHICS_H_
#define U2_GRAPHICS_H_

extern int window_width;
extern int window_height;

int init_graphics(char *, int, int);
void projection(void);
void swap_buffers(void);
void clear_screen(void);
void poll_event(void);

int adjust_framerate(void);

#endif
