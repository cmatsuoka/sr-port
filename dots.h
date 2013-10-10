#ifndef __KICK_H
#define __KICK_H

struct dot {
        int     x;
        int     y;
        int     z;
        int     old1;
        int     old2;
        int     old3;
        int     old4;
        int     yadd;
};


void draw_dot(struct dot *);
void poll_event(void);

#endif
