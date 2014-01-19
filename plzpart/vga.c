#include <string.h>
#include "common.h"

//var g_au8FrameBuffer = new Uint8Array(FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT);
static int plane_select[4] = { 0, 0, 0, 0 };
static int line_compare = 0;
static int hscroll_offset = 0;
static uint8_t palette[256 * 3];

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define FRAME_BUFFER_WIDTH 320
#define FRAME_BUFFER_HEIGHT 480

uint8_t fb[SCREEN_WIDTH * SCREEN_HEIGHT];


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
		if (!plane_select[i]) {
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

	copper1();
	copper2();

	int nFirstLineIndex = (line_compare + 1);

	// Plot the palettised frame buffer.
	int fb_offs = 0;

	for (y = nFirstLineIndex; y < SCREEN_HEIGHT; y++) {
		for (x = 0; x < SCREEN_WIDTH; x++) {
			//ASSERT(x + hscroll_offset < FRAME_BUFFER_WIDTH);
			int r, g, b;
			int idx = fb[fb_offs + x + hscroll_offset] * 3;

			r = palette[idx++];
			g = palette[idx++];
			b = palette[idx];

			// [NK 12/1/2014] VGA colours range from 0 - 63 inclusive, but
			// [NK 12/1/2014] SDL colours range from 0 - 255 inclusive, so
			// [NK 12/1/2014] account for this here.
#if 0
			r &= 63;
			g &= 63;
			b &= 63;

			r *= 255;
			r /= 63;
			g *= 255;
			g /= 63;
			b *= 255;
			b /= 63;

			SDL_SetPixelColours(x, y, r, g, b);
#endif

			int ofs = (y * 320 + x) * 3;
			image[ofs++] = (r << 2) & 0xff;
			image[ofs++] = (g << 2) & 0xff;
			image[ofs]   = (b << 2) & 0xff;
		}

		fb_offs += FRAME_BUFFER_WIDTH;
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
