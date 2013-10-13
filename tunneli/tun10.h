#ifndef TUN10_H_
#define TUN10_H_

struct bc {		/* each pixel */
	float x;
	float y;
};

struct rengas {		/* ring */
	float x, y;
	int c;
};

extern float sinit[4096];
extern float cosit[2048];

void draw_pixel(float, float, int);
void init_opengl(int, int);
void ballgen(void);
void singen(void);

#endif
