#include <stdio.h>
#include "bootpack.h"

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;

	init_gdtidt();
	init_pic();
	io_sti();	// now we can interrupt CPU after initialization of IDT/PIC
	
	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	char s[40], mcursor[256];
	int mx, my;
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_DARKSKY);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_WHITE, s);

	io_out8(PIC0_IMR, 0xf9); // allow PIC1 and keyboard
	io_out8(PIC1_IMR, 0xef); // allo mouse

	for(;;) {
		io_hlt();
	}
}


