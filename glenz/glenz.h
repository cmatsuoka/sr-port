#ifndef U2_GLENZ_H_
#define U2_GLENZ_H_

void cglenzdone(void);
void zoomer2(void);
void zoomer1(char *);

void init_opengl(int, int);
void draw_poly(int *);
void draw_fc(void);
void setrgb(int, int, int, int);
void getrgb(int, char *);

#endif
