#include "common.h"

int frame_count = 0;
int cop_drop = 0;
uint8_t *cop_pal;
int do_pal = 0;
int cop_start = 0;
int cop_scrl = 0;
int cop_plz = 1;

static int dtau[65];

uint8_t fadepal[768 * 2];
uint8_t *cop_fadepal;

extern int l1, l2, l3, l4;
extern int k1, k2, k3, k4;
extern int il1, il2, il3, il4;
extern int ik1, ik2, ik3, ik4;
extern int ttptr;


void pompota(void);
void moveplz(void);
void do_drop(void);
void initpparas(void);


int init_copper()
{
	int ccc;

	for (ccc = 0; ccc < 65; ccc++) {
		dtau[ccc] = ccc * ccc / 4 * 43 / 128 + 60;
	}

	return 0;
}

int close_copper()
{
	return 0;
}

// [nk] just before retrace
void copper1()
{
	// There is also assembly code to set the first pixel of
	// display memory here, but it may not be necessary.

	vga_set_hscroll_offset(cop_scrl);
}

// [nk] in retrace
void copper2()
{
	// [nk] Don't think this is used.
	frame_count++;

	if (do_pal != 0) {
		do_pal = 0;
		vga_upload_palette(cop_pal);
	}

	pompota();
	moveplz();

	if (cop_drop != 0)
	{
		do_drop();
	}
}

void pompota()
{

	// [NK 18/1/2014] Disable this for now, as it looks a bit jittery.
	return;

#if 0
	// [NK] This function toggles the horizontal split point every frame 
	// [NK] between line 60 and 61, and toggles the horizontal offset between 0 and 4.
	// [NK] I think the original intention of this code was to give more variation
	// [NK] cheaply, almost like a cheap alpha blend between two plasmas, but
	// [NK] it seems to cause a lot of flicker in the port. Perhaps more precise timing is
	// [NK] required to give the original effect? In any case I disabled this code
	// [NK] temporarily.

	vga_set_line_compare(60);
	cop_scrl = 4;

	pompi++;

	if ((pompi & 1) != 0)
	{
		// [NK 12/1/2014] Moving the starting line up and down each alternate frame
		// [NK 12/1/2014] doesn't look good in windowed mode.
		// [NK 13/1/2014] Seems to work okay in fullscreen mode though.
		//vga_set_line_compare(61);
		cop_scrl = 0;
	}
#endif
}

void moveplz()
{
	k1 += -3;
	k1 &= 4095;
	k2 += -2;
	k2 &= 4095;
	k3 += 1;
	k3 &= 4095;
	k4 += 2;
	k4 &= 4095;
	l1 += -1;
	l1 &= 4095;
	l2 += -2;
	l2 &= 4095;
	l3 += 2;
	l3 &= 4095;
	l4 += 3;
	l4 &= 4095;
}

void do_drop()
{
	cop_drop++;

	if (cop_drop <= 64) {
		vga_set_line_compare(dtau[cop_drop]);
	} else {
		//@@over

		int bShouldFade = 0;

#if 0
		// [NK 18/1/2014] Hack for looping back to the first plasma.
		if ((cop_drop == 65) && (ttptr == 0)) {
			cop_drop = 128;
		}
#endif

		if (cop_drop >= 256) {
		} else if (cop_drop >= 128) {
			bShouldFade = 1;
		} else if (cop_drop > 96) {
		} else /*if (cop_drop > 64) */{
			bShouldFade = 1;
		}

		if (bShouldFade) {
			cop_pal = fadepal;
			do_pal = 1;

			if (cop_drop == 65) {
				vga_set_line_compare(400);
				initpparas();
			} else {
				int i, ccc;

				vga_set_line_compare(60);

				// [NK 9/1/2014] Fade the palette using 8.8 fixed point numbers. 

				uint8_t* pcop_fadepal = cop_fadepal;
				uint8_t* pfadepal = fadepal;

				for (i = 0; i < (768 / 16); i++) {
					for (ccc = 0; ccc < 16; ccc++) {
						uint8_t al = pcop_fadepal[ccc*2];
						uint8_t ah = pcop_fadepal[(ccc*2) + 1];

						uint8_t oldval = pfadepal[ccc + 768];
						pfadepal[ccc + 768] += al;
						uint8_t newval = pfadepal[ccc + 768];
						uint8_t carry = 0;
						if (newval < oldval) {
							carry = 1;
						}

						pfadepal[ccc] += ah + carry;
					}

					pcop_fadepal += 32;
					pfadepal += 16;
				}
			}
		} else {
			cop_drop = 0;
		}
	}
}

void initpparas()
{
	l1 = il1;
	l2 = il2;
	l3 = il3;
	l4 = il4;

	k1 = ik1;
	k2 = ik2;
	k3 = ik3;
	k4 = ik4;
}

