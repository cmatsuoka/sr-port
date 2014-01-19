#include "common.h"
#include "tweak.h"

void tw_opengraph()
{
}

void tw_opengraph2()
{
}

void tw_setrgbpalette(int pal, int r, int g, int b)
{
	vga_set_palette_entry(pal, r, g, b);
}
