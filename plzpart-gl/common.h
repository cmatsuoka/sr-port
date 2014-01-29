
#include <stdint.h>


uint16_t psini_read16(int);
uint8_t psini_read8(int);
void psini_write16(int, uint16_t);
void psini_write8(int, uint8_t);

void vga_select_bitplanes_02(void);
void vga_select_bitplanes_13(void);
void vga_select_bitplanes_0123(void);
void vga_write32(int, int);
void vga_set_line_compare(int);
void vga_set_palette_entry(int, int, int, int);
void vga_show_framebuffer(void);
void vga_set_hscroll_offset(int);
void vga_upload_palette(uint8_t *);

void initvect(void);

int init_copper(void);
int close_copper(void);
void copper1(void);
void copper2(void);
