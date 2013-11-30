#ifndef U2_GRAPHICS_H_
#define U2_GRAPHICS_H_

int init_graphics(char *, int, char **);
void projection(void);
void swap_buffers(void);
void clear_screen(void);
void poll_event(void);

int adjust_framerate(void);
void set_fps(float);

#endif
