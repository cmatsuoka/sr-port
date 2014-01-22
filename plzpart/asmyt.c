#include "common.h"

static uint16_t selfmod[5][84];
extern char *psini;

#define PSINI_OFFSET	0
#define LSINI4_OFFSET	16384
#define LSINI16_OFFSET	(16384 + 2 * 8192)


int plzline(int y, int vseg)
{
	// vseg represented a segment, so multiply by sixteen (shift left by 4)
	// to convert into an offset.
	int nVgaYOffset = vseg << 4;

	int cccTable[] = {
		3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18,
		17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28, 35,
		34, 33, 32, 39, 38, 37, 36, 43, 42, 41, 40, 47, 46, 45, 44,
		51, 50, 49, 48, 55, 54, 53, 52, 59, 58, 57, 56, 63, 62, 61,
		60, 67, 66, 65, 64, 71, 70, 69, 68, 75, 74, 73, 72, 79, 78,
		77, 76, 83, 82, 81, 80
	};
	uint8_t ah = 0;
	uint8_t al = 0;
	uint32_t eax = 0;
	int i;

	for (i = 0; i < 84; i++) {
		int ccc = cccTable[i];
		uint16_t offs;
		int bx;

		if ((ccc & 1) == 1) {
			// within lsini16
			offs = (y * 2) + selfmod[2][ccc];
			bx = *(short *)&psini[offs];

			// within psini
			offs = bx + selfmod[1][ccc];
			ah = psini[offs];

			// within lsini4
			offs = (y * 2) + selfmod[4][ccc];
			bx = *(short *)&psini[offs];

			// within psini
			offs = bx + (y * 2) + selfmod[3][ccc];
			ah += psini[offs];
		} else {
			offs = (y * 2) + selfmod[2][ccc];
			bx = *(short *)&psini[offs];

			offs = bx + selfmod[1][ccc];
			al = psini[offs];

			offs = (y * 2) + selfmod[4][ccc];
			bx = *(short *)&psini[offs];

			offs = bx + (y * 2) + selfmod[3][ccc];
			al += psini[offs];
		}

		if ((ccc & 3) == 2) {
			eax = (ah << 8) | al;
			eax <<= 16;
		}

		if ((ccc & 3) == 0) {
			eax |= (ah << 8) | al;
			vga_write32(nVgaYOffset + ccc, eax);
		}
	}

	return 0;
}

int setplzparas(int c1, int c2, int c3, int c4)
{
	int ccc;

	for (ccc = 0; ccc < 84; ccc++) {
		uint16_t lc1, lc2, lc3, lc4;

		lc1 = c1 + PSINI_OFFSET + (ccc * 8);
		selfmod[1][ccc] = lc1;

		lc2 = (c2 * 2) + LSINI16_OFFSET - (ccc * 8) + (80 * 8);
		selfmod[2][ccc] = lc2;

		lc3 = c3 + PSINI_OFFSET - (ccc * 4) + (80 * 4);
		selfmod[3][ccc] = lc3;

		lc4 = (c4 * 2) + LSINI4_OFFSET + (ccc * 32);
		selfmod[4][ccc] = lc4;
	}

	return 0;
}

int set_plzstart(int y)
{
	vga_set_line_compare(y);

	return 0;
}
