#include "bootpack.h"

extern struct KEYBUF keybuf;

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;

	init_gdtidt();
	init_pic();
	io_sti();	// now we can interrupt CPU after initialization of IDT/PIC
	
	io_out8(PIC0_IMR, 0xf9); // allow PIC1 and keyboard
	io_out8(PIC1_IMR, 0xef); // allo mouse

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

	int i, j;

	for(;;) {
		io_cli();
		if (keybuf.next == 0) {
			io_stihlt();
		} else {
			i = keybuf.data[0];
			keybuf.next--;
			for (j = 0; j < keybuf.next; j++)
				keybuf.data[j] = keybuf.data[j+1];
			io_sti();
			sprintf(s, "%02X", i);
			boxfill8(binfo->vram, binfo->scrnx, COL8_DARKSKY, 0, 16, 15, 31);
			putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_WHITE, s);
		}
	}
}


