/*
 * Written by Nick Kovac with minor changes by Claudio Matsuoka
 */

#include <string.h>
#include "common.h"

static int plane_select[4] = { 0, 0, 0, 0 };
static int line_compare = 0;
static int hscroll_offset = 0;
static uint8_t palette[256 * 3];

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 400
#define FRAME_BUFFER_WIDTH 384
#define FRAME_BUFFER_HEIGHT 400

uint8_t fb[FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT];


void vga_select_bitplanes_02()
{
	plane_select[0] = 1;
	plane_select[1] = 0;
	plane_select[2] = 1;
	plane_select[3] = 0;
}

void vga_select_bitplanes_13()
{
	plane_select[0] = 0;
	plane_select[1] = 1;
	plane_select[2] = 0;
	plane_select[3] = 1;
}

void vga_select_bitplanes_0123()
{
	plane_select[0] = 1;
	plane_select[1] = 1;
	plane_select[2] = 1;
	plane_select[3] = 1;
}

void vga_write32(int offset, int val)
{
	int i, j;

	// For each selected plane, calculate 4 pixel offsets and write 4 bytes.
	// offset = 0, 4, 8, etc

	for (i = 0; i < 4; i++) {
		if (plane_select[i]) {
			for (j = 0; j < 4; j++) {
				// Get byte to write.
				uint8_t b = (val >> (j * 8));

				// Calculate offset.
				int fb_offs = i + (offset + j) * 4;

				// Write pixel.
				fb[fb_offs] = b;
			}
		}
	}
}

void vga_set_line_compare(int y)
{
	line_compare = y;
}

void vga_set_palette_entry(int index, int r, int g, int b)
{
	index *= 3;
	palette[index++] = r;
	palette[index++] = g;
	palette[index] = b;
}

extern unsigned char *image;	// The OpenGL background image texture

void vga_show_framebuffer()
{
	int x, y;

	// Clear the screen.
	memset(image, 0, FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * 3);

	copper1();
	copper2();

	int nFirstLineIndex = (line_compare + 1);

	// Plot the palettised frame buffer.
	uint8_t *ptr = fb;

	for (y = nFirstLineIndex; y < SCREEN_HEIGHT; y++) {
		for (x = 0; x < SCREEN_WIDTH; x++) {
			//ASSERT(x + hscroll_offset < FRAME_BUFFER_WIDTH);
			uint8_t r, g, b;
			int idx = ptr[x + hscroll_offset] * 3;

			r = palette[idx++];
			g = palette[idx++];
			b = palette[idx];

			int ofs = (y * 320 + x) * 3;
			image[ofs++] = r << 2;
			image[ofs++] = g << 2;
			image[ofs]   = b << 2;
		}

		ptr += FRAME_BUFFER_WIDTH;
	}
}

void vga_set_hscroll_offset(int offset)
{
	hscroll_offset = offset;
}

void vga_upload_palette(uint8_t *pal)
{
	memcpy(palette, pal, 256 * 3);
}
