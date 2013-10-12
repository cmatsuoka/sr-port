#ifndef DOTS_H_
#define DOTS_H_

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


void draw_dot(struct dot *);
void poll_event(void);

#endif
