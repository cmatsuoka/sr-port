#include <math.h>
#include "common.h"

int frame_count = 0;
int cop_drop = 0;
int cop_pal = 0;
int do_pal = 0;
int cop_start = 0;
int cop_scrl = 0;
int cop_plz = 1;

static int dtau[65];

uint8_t fadepal[768 * 2];
char *cop_fadepal;

extern int l1, l2, l3, l4;
extern int k1, k2, k3, k4;
extern int il1, il2, il3, il4;
extern int ik1, ik2, ik3, ik4;
extern int ttptr;

extern char *pals;


int init_copper()
{
	int ccc;

	for (ccc = 0; ccc < 65; ccc++) {
		dtau[ccc] = floor(1.0 * ccc * ccc / 4 * 43 / 128 + 60);
	}

	return 0;
}

int close_copper()
{
	return 0;
}

void pompota()
{

	// [NK 18/1/2014] Disable this for now, as it looks a bit jittery.
	return;

#if 0
	// [nk] This function toggles the horizontal split point every frame
	// [nk] between line 60 and 61, along with the horizontal offset.
	// [nk] (since set_plzstart == 60, it's splitting at the top of the plasma)
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

void do_drop()
{
	cop_drop++;

	if (cop_drop <= 64) {
		vga_set_line_compare(dtau[cop_drop]);
	} else {
		//@@over

		int bShouldFade = 0;

		// [NK 18/1/2014] Hack for looping back to the first plasma.
		if ((cop_drop == 65) && (ttptr == 0)) {
			cop_drop = 128;
		}

		if (cop_drop >= 256) {
		} else if (cop_drop >= 128) {
			bShouldFade = 1;
		} else if (cop_drop > 96) {
		} else /*if (cop_drop > 64) */{
			bShouldFade = 1;
		}

		if (bShouldFade) {
			// [NK 15/1/2014] cop_pal always points to fadepal, so just upload fadepal.
			//cop_pal = fadepal;
			do_pal = 1;

			if (cop_drop == 65) {
				vga_set_line_compare(400);
				initpparas();
			} else {
				int i, ccc;
				int cop_fadepal_idx = 0;
				int fadepal_idx = 0;

				vga_set_line_compare(60);

				// [NK 9/1/2014] I think it's using 8.8 fixed point numbers to fade the palette.

				for (i = 0; i < (768 / 16); i++) {
					for (ccc = 0; ccc < 16; ccc++) {
						// var al = cop_fadepal_ReadByte(cop_fadepal_idx + (ccc * 2));
						// al &= 0xFF;
						// var ah = cop_fadepal_ReadByte(cop_fadepal_idx + (ccc * 2) + 1);
						// ah &= 0xFF;
						// [NK 17/1/2014] Read cop_fadepal as words, rather than bytes,
						// [NK 17/1/2014] to avoid endian issues.
						int ax = *(short *)&cop_fadepal[cop_fadepal_idx + (ccc * 2)];
						int al = ax & 0xFF;
						int ah = (ax >> 8) & 0xFF;

						int nOldValue = fadepal[fadepal_idx + ccc + 768] & 0xff;

						int t = fadepal[fadepal_idx + ccc + 768];
						t &= 0xFF;
						t += al;
						t &= 0xFF;
						fadepal[fadepal_idx + ccc + 768] = t;

						int nNewValue = fadepal[fadepal_idx + ccc + 768];
						nNewValue &= 0xFF;

						int carry = 0;

						if (nNewValue < nOldValue) {
							carry = 1;
						}

						t = fadepal[fadepal_idx + ccc];
						t &= 0xFF;
						t += ah + carry;
						t &= 0xFF;
						fadepal[fadepal_idx + ccc] = t;
					}

					cop_fadepal_idx += 32;
					fadepal_idx += 16;
				}
			}
		} else {
			cop_drop = 0;
		}
	}
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
		// [NK 15/1/2014] cop_pal always points to fadepal, so just upload fadepal.
		//vga_upload_palette(cop_pal);
		vga_upload_palette(fadepal);
	}

	pompota();
	moveplz();

	if (cop_drop != 0)
	{
		do_drop();
	}
}

