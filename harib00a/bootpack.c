void io_hlt(void);
void io_cli(void);
void io_cut8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfont8(char*, int, int, int, char, char*);

#define COL8_BLACK		0
#define COL8_VIVRED		1
#define COL8_VIVGREEN		2
#define COL8_VIVYELLOW		3
#define COL8_VIVBLUE		4
#define COL8_VIVPURPLE		5
#define COL8_VIVSKY		6
#define COL8_WHITE		7
#define COL8_VIVGRAY		8
#define COL8_DARKRED		9
#define COL8_DARKGREEN		10
#define COL8_DARKYELLOW		11
#define COL8_DARKBLUE		12
#define COL8_DARKPURPLE	13
#define COL8_DARKSKY		14
#define COL8_DARKGRAY		15


struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
	extern char hankaku[4096];

	init_palette();
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

	putfont8(binfo->vram, binfo->scrnx, 8, 8, COL8_WHITE, hankaku + 'A' * 16);
	putfont8(binfo->vram, binfo->scrnx, 16, 8, COL8_WHITE, hankaku + 'B' * 16);
	putfont8(binfo->vram, binfo->scrnx, 24, 8, COL8_WHITE, hankaku + 'C' * 16);
	putfont8(binfo->vram, binfo->scrnx, 40, 8, COL8_WHITE, hankaku + '1' * 16);
	putfont8(binfo->vram, binfo->scrnx, 48, 8, COL8_WHITE, hankaku + '2' * 16);
	putfont8(binfo->vram, binfo->scrnx, 56, 8, COL8_WHITE, hankaku + '3' * 16);

	for(;;) {
		io_hlt();
	}
}

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	// 0: black
		0xff, 0x00, 0x00,	// 1: vivid red
		0x00, 0xff, 0x00,	// 2: vivid green
		0xff, 0xff, 0x00,	// 3: vivid yellow
		0x00, 0x00, 0xff,	// 4: vivid blue
		0xff, 0x00, 0xff,	// 5: vivid purple
		0x00, 0xff, 0xff,	// 6: vivid sky
		0xff, 0xff, 0xff,	// 7: white
		0xc6, 0xc6, 0xc6,	// 8: vivid gray
		0x84, 0x00, 0x00,	// 9: dark red
		0x00, 0x84, 0x00,	// 10: dark green
		0x84, 0x84, 0x00,	// 11: dark yellow
		0x00, 0x00, 0x84,	// 12: dark blue
		0x84, 0x00, 0x84,	// 13: dark purple
		0x00, 0x84, 0x84,	// 14: dark sky
		0x84, 0x84, 0x84	// 15: dark gray
	};
	set_palette(0, 15, table_rgb);
	return;

}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();
	io_cli();
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++) {
			vram[y * xsize + x] = c;
		}
	}
	return;
}

void init_screen(char *vram, int xsize, int ysize)
{
	boxfill8(vram, xsize, COL8_DARKSKY,  0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_VIVGRAY,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_WHITE,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_VIVGRAY,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(vram, xsize, COL8_WHITE,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_WHITE,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_DARKGRAY,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_DARKGRAY, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_BLACK,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_BLACK, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(vram, xsize, COL8_DARKGRAY, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_DARKGRAY, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_WHITE, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_WHITE, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) p[0] = c;
		if ((d & 0x40) != 0) p[1] = c;
		if ((d & 0x20) != 0) p[2] = c;
		if ((d & 0x10) != 0) p[3] = c;
		if ((d & 0x08) != 0) p[4] = c;
		if ((d & 0x04) != 0) p[5] = c;
		if ((d & 0x02) != 0) p[6] = c;
		if ((d & 0x01) != 0) p[7] = c;
	}
	return;
}
