#include <stdio.h>
#include "bootpack.h"

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;

	init_gdtidt();
	
	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

	char mcursor[256];
	int mx, my;
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_DARKSKY);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

	for(;;) {
		io_hlt();
	}
}


