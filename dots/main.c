#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include "graphics.h"
#include "dots.h"

#define MAXDOTS 1024
#define BOTTOM 8000

int timer=30000;
int dropper,repeat;
int frame=0;
int rota=-1*64;
int fb=0;
int rot=0,rots=0;
int a,b,c,d,i,j,mode;
int grav,gravd;
int f=0;
int bpmin = 30000;
int bpmax = -30000;


int dottaul[1024];

struct dot dot[MAXDOTS];

float rotsin, rotcos;
int gravitybottom = BOTTOM;
int gravity = 0;
int dotnum;
int gravityd = 16;
int rows[200];
#if 0
int depthtable1[128];
int depthtable2[128];
int depthtable3[128];
int depthtable4[128];
#endif

extern float colorshift;
extern int sin1024[];

#if 0
static int isin(int deg)
{
        return(sin1024[deg&1023]);
}

static int icos(int deg)
{
        return(sin1024[(deg+256)&1023]);
}
#endif

static float isin(int deg)
{
	return sin(M_PI * deg / 512) * 255;
}

static float icos(int deg)
{
	return cos(M_PI * deg / 512) * 255;
}


static void setup_dots()
{
        dotnum=512;
        for(a=0;a<dotnum;a++)  dottaul[a]=a;
        for(a=0;a<500;a++)
        {
                b=rand()%dotnum;
                c=rand()%dotnum;
                d=dottaul[b];
                dottaul[b]=dottaul[c];
                dottaul[c]=d;
        }
        {
                dropper=22000;
                for(a=0;a<dotnum;a++)
                {
                        dot[a].x=0;
                        dot[a].y=2560-dropper;
                        dot[a].z=0;
                        dot[a].yadd=0;
                }
                mode=7;
                grav=3;
                gravd=13;
                gravitybottom=8105;
                i=-1;
        }
        for(a=0;a<500;a++)
        { // scramble
                b=rand()%dotnum;
                c=rand()%dotnum;
                d=dot[b].x; dot[b].x=dot[c].x; dot[c].x=d;
                d=dot[b].y; dot[b].y=dot[c].y; dot[c].y=d;
                d=dot[b].z; dot[b].z=dot[c].z; dot[c].z=d;
        }
        for(a=0;a<200;a++) rows[a]=a*320;

        for(a=0;a<128;a++)
        {
                c=a-(43+20)/2;
                c=c*3/4;
                c+=8;
                if(c<0) c=0; else if(c>15) c=15;
                c=15-c;
#if 0
                depthtable1[a]=0x202+0x04040404*c;
                depthtable2[a]=0x02030302+0x04040404*c;
                depthtable3[a]=0x202+0x04040404*c;
                //depthtable4[a]=0x02020302+0x04040404*c;
#endif
        }


}


static void update_dots()
{
//setborder(0);
//repeat=dis_waitb();

//if(frame>2300) setpalette(pal2);

//setborder(1);
#if 0
if(dis_indemo())
{
	a=dis_musplus();
	if(a>-4 && a<0) break;
}
#endif
	frame++;
	if(frame==500) f=0;
	i=dottaul[j];
	j++; j%=dotnum;
	if(frame<500)
	{
		dot[i].x=isin(f*11)*40;
		dot[i].y=icos(f*13)*10-dropper;
		dot[i].z=isin(f*17)*40;
		dot[i].yadd=0;
	}
	else if(frame<900)
	{
		dot[i].x=icos(f*15)*55;
		dot[i].y=dropper;
		dot[i].z=isin(f*15)*55;
		dot[i].yadd=-260;
	}
	else if(frame<1700)
	{	
		a=sin1024[frame&1023]/8;
		dot[i].x=icos(f*66)*a;
		dot[i].y=8000;
		dot[i].z=isin(f*66)*a;
		dot[i].yadd=-300;
	}
	else if(frame<2360)
	{
		/*
		a=rand()/128+32;
		dot[i].y=8000-a*80;
		b=rand()&1023;
		a+=rand()&31;
		dot[i].x=sin1024[b]*a/3+(a-50)*7;
		dot[i].z=sin1024[(b+256)&1023]*a/3+(a-40)*7;
		dot[i].yadd=300;
		if(frame>1640 && !(frame&31) && grav>-2) grav--;
		*/
		dot[i].x=(rand() % 0x7fff) -16384;
		dot[i].y=8000-(rand() % 0x7fff) /2;
		dot[i].z=(rand() % 0x7fff) -16384;
		dot[i].yadd=0;
		if(frame>1900 && !(frame&31) && grav>0) grav--;
	}
	else if(frame<2400)
	{
		colorshift = 1.0f * (frame - 2360) / 32;
		if (colorshift > 1.0f)
			colorshift = 1.0f;

#if 0
		a=frame-2360;
		for(b=0;b<768;b+=3)
		{
			c=pal[b+0]+a*3;
			if(c>63) c=63;
			pal2[b+0]=c;
			c=pal[b+1]+a*3;
			if(c>63) c=63;
			pal2[b+1]=c;
			c=pal[b+2]+a*4;
			if(c>63) c=63;
			pal2[b+2]=c;
		}
#endif
	}
	else if(frame<2440)
	{
		colorshift = 1.0f + 1.0f * (frame - 2400) / 32;
		if (colorshift > 2.0f)
			colorshift = 2.0f;
#if 0
		a=frame-2400;
		for(b=0;b<768;b+=3)
		{
			c=63-a*2;
			if(c<0) c=0;
			pal2[b+0]=c;
			pal2[b+1]=c;
			pal2[b+2]=c;
		}
#endif
	}
	if(dropper>4000) dropper-=100;
	rotcos=icos(rot)*64; rotsin=isin(rot)*64;
	rots+=2;
	if(frame>1900) 
	{
		rot+=rota/64;
		rota--;
	}
	else rot=isin(rots);
	f++;
	gravity=grav;
	gravityd=gravd;
}

static void draw_dots()
{
	int i;

	for (i = 0; i < dotnum; i++) {
		draw_dot(&dot[i]);
	}
}

int main()
{
	int width, height;

	srand(0xfcfc);

	width = window_width;
	height = window_height;

	if (init_graphics("DOTS", width, height) < 0) {
		fprintf(stderr, "can't init graphics\n");
		exit(1);
	}

	init_opengl(width, height);
	setup_dots();

	projection();

	while(frame<2450) {
		poll_event();
		update_dots();
		clear_screen();
		draw_dots();
		swap_buffers();
	}

	return 0;
}

