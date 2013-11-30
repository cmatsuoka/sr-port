#include <stdio.h>
#include <string.h>
#include <math.h>
#include "graphics.h"
#include "tun10.h"

const int veke = 1060;    /* frame count to exit */

struct rengas putki[102];        /* "pipe" */
struct bc pcalc[138][64];        /* pixel position */

//int aa1[100];
//int rows[200];
//int aa2[200];

float sinit[4096];
float cosit[2048];

int sade[102];
int frame;
int bor;
int frames;

float color[256][3];

void setrgb(int c, int r, int g, int b)
{
    color[c][0] = (float)r / 64;
    color[c][1] = (float)g / 64;
    color[c][2] = (float)b / 64;
}


#if 0
void waitr;
{
        setrgb(0,0,0,0);
        asm
        mov        bx,1
        int        0fch
        mov     frames,ax
        }
        if mem[$40:$17] and 16 = 16 then setrgb(0,0,0,15);
}

function dis_exit:boolean;
var   a : byte;
{
        asm
        mov        bx,2
        int        0fch
        mov     a,al
        }
        if a = 0 then dis_exit = false else dis_exit = true;
}
#endif

//int oldpos[7500];
//int op;
//int ry;

int x, y, z;
int x1, _y1;
int a;
//FILE *f;

int sx, sy;
float _bx, by;
int br;
int bbc;                /* color */
struct bc *pcp;
//int pc;
//int mx, my;
//int addi, yvalue;
//char ch;
//int oo;
//int flip;
int quit;
int sync;

#define false 0
#define true (!false)

int main(int argc, char **argv)
{
    frames = 1;

    x = 0;y = 0;z = 0;x1 = 0;_y1 = 0;a = 0;sx = 0;sy = 0;

    quit = false;

    ballgen();
    singen();

#if 0
    for (x = 0; x < 100; x++)
        aa1[x] = 64000;
    memcpy(aa1,aa2,sizeof(aa1));

    for (x = 0; x < 200; x++)
        rows[x] = x*320;
#endif

    /*move(mem[seg(sini):ofs(sini)],sinit,4097*2);
    move(mem[seg(sini):ofs(sini)+4097*2],cosit,2048*2);
    move(mem[seg(tun):ofs(tun)],pcalc,sizeof(pcalc));*/

    if (init_graphics("Tunneli", argc, argv) < 0) {
        fprintf(stderr, "Can't init graphics\n");
        return -1;
    };

    init_opengl();

/*
    for x = 0 to 64 do setrgb(64+x,(64-x) *4 div 7,(64-x) * 2 div 3,64-x);
    for x = 0 to 64 do setrgb(128+x,(64-x) div 3,(64-x) div 2,(64-x) * 2 div 3);
*/
    for (x = 0; x <= 64; x++) setrgb(64+x ,(64-x),(64-x),(64-x));
    for (x = 0; x <= 64; x++) setrgb(128+x,(64-x) *3 / 4,(64-x) *3 / 4,(64-x) *3 / 4);

    setrgb(68,0,0,0);
    setrgb(132,0,0,0);


    setrgb(255,0,63,0);

    for (x = 0; x <= 100; x++) {
        putki[x].x = 0;
        putki[x].y = 0;
        putki[x].c = 0;
    }

  sx = 0;
  sy = 0;

  //pc = 60;
  //addi = 40;
  //flip = 0;
  //bor = 0;
  frame = 0;
  quit = false;

  //waitr;
  for (z = 0; z < 100; z++) sade[z] = 16384 / ((z*7)+95);

  do {
    //waitr;
    //ry = 0;

    poll_event();
    clear_screen();
    
    for (x = 80; x >= 4; x--) {
     _bx = putki[x].x-putki[5].x;
      by = putki[x].y-putki[5].y;
      br = sade[x];
      bbc = putki[x].c+(int)((float)x / 1.3);   /* pixel color */
      pcp = &pcalc[br][0];

      if (bbc >= 64) {
        int i;

        for (i = 0; i < 64; i++) {
           float xpos, ypos;

           xpos = pcp[i].x + _bx;
           if (xpos >= 0 && xpos <= 319) {
              ypos = pcp[i].y + by;
              if (ypos >= 0 && ypos <= 199)
                 draw_pixel(xpos, ypos, bbc);
           }
        }
      }
    }

    swap_buffers();

    for (sync = 1; sync <= frames; sync++) {
      putki[100].x = cosit[sy & 2047]-sinit[sy*3 & 4095]/*-cosit[sx & 2047]*/;
      putki[100].y = sinit[sx*2 & 4095]-cosit[sx & 2047]+sinit[y & 4095];
      //memmove(&putki[0],&putki[1],&putki[100]-&putki[0]);
      memmove(&putki[0],&putki[1],100 * sizeof (struct rengas));
      sy++;
      sx++;
 
      if ((sy & 15) > 7) putki[99].c = 128; else putki[99].c = 64;
      if (frame >= veke-102) putki[99].c = 0;
      if (frame == veke) quit = true; else frame++;
    }
  } while (!quit);

  return 0;
}
