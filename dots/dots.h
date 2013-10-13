#ifndef SR_DOTS_H_
#define SR_DOTS_H_

struct dot {
        float   x;
        float   y;
        float   z;
        int     old1;
        int     old2;
        int     old3;
        int     old4;
        float   yadd;
};


int init_opengl(int, int);
void draw_dot(struct dot *);

#endif
