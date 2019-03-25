#include "bootpack.h"

void make_window8(unsigned char *buf, int xsize, int ysize, char *title);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);

struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};

void task_b_main(void)
{
	struct FIFO32 fifo;
	struct TIMER *timer;
	int i, fifobuf[128];

	fifo32_init(&fifo, 128, fifobuf);
	timer = timer_alloc();
	timer_init(timer, &fifo, 1);
	timer_settime(timer, 500);

	for (;;) {
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_stihlt();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == 1) {
				taskswitch3();
			}
		}
	}
}

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
	struct FIFO32 fifo;
	char s[40];
	int fifobuf[128];
	int mx, my, i, count = 0;
	int cursor_x, cursor_c;
	int task_b_esp;
	struct MOUSE_DEC mdec;
	unsigned int memtotal;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct SHTCTL *shtctl;
	struct SHEET *sht_back, *sht_mouse, *sht_win;
	struct TIMER *timer, *timer2, *timer3;
	unsigned char *buf_back, *buf_mouse, *buf_win;
	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.'
	};

	init_gdtidt();
	init_pic();
	io_sti();	// now we can interrupt CPU after initialization of IDT/PIC
	
	fifo32_init(&fifo, 128, fifobuf);
	init_pit();
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);
	
	io_out8(PIC0_IMR, 0xf8); // allow timer, PIC1 and keyboard
	io_out8(PIC1_IMR, 0xef); // allo mouse

	timer = timer_alloc();
	timer_init(timer, &fifo, 10);
	timer_settime(timer, 1000);
	timer2 = timer_alloc();
	timer_init(timer2, &fifo, 3);
	timer_settime(timer2, 300);
	timer3 = timer_alloc();
	timer_init(timer3, &fifo, 1);
	timer_settime(timer3, 50);

	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000);
	memman_free(memman, 0x00400000, memtotal - 0x00400000);
	
	struct TSS32 tss_a, tss_b;
	tss_a.ldtr = 0;
	tss_a.iomap = 0x40000000;
	tss_b.ldtr = 0;
	tss_b.iomap = 0x40000000;


	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;

	set_segmdesc(gdt + 3, 103, (int) &tss_a, AR_TSS32);
	set_segmdesc(gdt + 4, 103, (int) &tss_b, AR_TSS32);

	task_b_esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024;

	load_tr(3 * 8);
	task_b_esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024;
	tss_b.eip = (int) &task_b_main;
	tss_b.eflags = 0x00000202; /* IF = 1; */
	tss_b.eax = 0;
	tss_b.ecx = 0;
	tss_b.edx = 0;
	tss_b.ebx = 0;
	tss_b.esp = task_b_esp;
	tss_b.ebp = 0;
	tss_b.esi = 0;
	tss_b.edi = 0;
	tss_b.es = 1 * 8;
	tss_b.cs = 2 * 8;
	tss_b.ss = 1 * 8;
	tss_b.ds = 1 * 8;
	tss_b.fs = 1 * 8;
	tss_b.gs = 1 * 8;

	init_palette();
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	sht_back = sheet_alloc(shtctl);
	sht_mouse = sheet_alloc(shtctl);
	sht_win = sheet_alloc(shtctl);
	buf_back = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	buf_mouse = (unsigned char *) memman_alloc(memman, 256);
	buf_win = (unsigned char *) memman_alloc_4k(memman, 160 * 52);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
	sheet_setbuf(sht_win, buf_win, 160, 52, -1);
	init_screen8(buf_back, binfo->scrnx, binfo->scrny);
	init_mouse_cursor8(buf_mouse, 99);
	make_window8(buf_win, 160, 52, "window");
	make_textbox8(sht_win, 8, 28, 144, 16, COL8_WHITE);
	sheet_slide(sht_back, 0, 0);
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	sheet_slide(sht_mouse, mx, my);
	sheet_slide(sht_win, 80, 72);
	sheet_updown(sht_back, 0);
	sheet_updown(sht_win, 1);
	sheet_updown(sht_mouse, 2);
	sprintf(s, "(%3d, %3d)", mx, my);
	putfonts8_asc_sht(sht_back, 0, 0, COL8_WHITE, COL8_DARKSKY, s, 10);
	sprintf(s, "memory %dMB   free : %dKB",
			memtotal / (1024 * 1024), memman_total(memman) / 1024);
	putfonts8_asc_sht(sht_back, 0, 32, COL8_WHITE, COL8_DARKSKY, s, 40);

	cursor_x = 8;
	cursor_c = COL8_WHITE;

	for(;;) {
		//sprintf(s, "%010d", timerctl.count);
		//putfonts8_asc_sht(sht_win, 40, 28, COL8_BLACK, COL8_VIVGRAY, s, 10);

		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_stihlt();
		} else {
			i = fifo32_get(&fifo);
			//sprintf(s, "%010d", i);
			//putfonts8_asc_sht(sht_back, 0, 0, COL8_BLACK, COL8_DARKSKY, s, 10);
			io_sti();
			if (256 <= i && i <= 511){
				// keyboard
				sprintf(s, "%02X", i - 256);
				putfonts8_asc_sht(sht_back, 0, 16, COL8_WHITE, COL8_DARKSKY, s, 2);
				if (i < 256 + 0x54) {
					if (keytable[i - 256] != 0 && cursor_x < 144) {
						s[0] = keytable[i - 256];
						s[1] = 0;
						putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_BLACK, COL8_WHITE, s, 1);
						cursor_x += 8;
					}
				}
				if (i == 256 + 0x0e && cursor_x > 8) {
					// backspace
					putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_BLACK, COL8_WHITE, " ", 1);
					cursor_x -= 8;
				}
				boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
				sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
			} else if (512 <= i && i <= 767) {
				if (mouse_decode(&mdec, i - 512) != 0) {
					// print data
					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0){
						s[1] = 'L';
					}
					if ((mdec.btn & 0x02) != 0){
						s[1] = 'R';
					}
					if ((mdec.btn & 0x04) != 0){
						s[1] = 'C';
					}
					putfonts8_asc_sht(sht_back, 32, 16, COL8_WHITE, COL8_DARKSKY, s, 15);

					// move mouse cursor
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 1) {
						mx = binfo->scrnx - 1;
					}
					if (my > binfo->scrny - 1) {
						my = binfo->scrny - 1;
					}
					sprintf(s, "(%3d, %3d)", mx, my);
					putfonts8_asc_sht(sht_back, 0, 0, COL8_WHITE, COL8_DARKSKY, s, 10);
					sheet_slide(sht_mouse, mx, my);
					if ((mdec.btn & 0x01) != 0) {
						// if left_button is pushed, mov sht_win
						sheet_slide(sht_win, mx - 80, my - 8);
					}
				}
			} else if (i == 10) {
				putfonts8_asc_sht(sht_back, 0, 64, COL8_WHITE, COL8_DARKSKY, "10[sec]", 7);
				taskswitch4();
			} else if (i == 3) {
				putfonts8_asc_sht(sht_back, 0, 80, COL8_WHITE, COL8_DARKSKY, "3[sec]", 6);
				count = 0;
			} else if (i <= 1) {
				if (i != 0) {
					timer_init(timer3, &fifo, 0);
					cursor_c = COL8_BLACK;
				} else {
					timer_init(timer3, &fifo, 1);
					cursor_c = COL8_WHITE;
				}
				timer_settime(timer3, 50);
				boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
				sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
			}
		}
	}
}

void make_window8(unsigned char *buf, int xsize, int ysize, char *title)
{
	static char closebtn[14][16] = {
		"OOOOOOOOOOOOOOO@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQQQ@@QQQQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"O$$$$$$$$$$$$$$@",
		"@@@@@@@@@@@@@@@@"
	};
	int x, y;
	char c;
	boxfill8(buf, xsize, COL8_VIVGRAY, 0,         0,         xsize - 1, 0        );
	boxfill8(buf, xsize, COL8_WHITE, 1,         1,         xsize - 2, 1        );
	boxfill8(buf, xsize, COL8_VIVGRAY, 0,         0,         0,         ysize - 1);
	boxfill8(buf, xsize, COL8_WHITE, 1,         1,         1,         ysize - 2);
	boxfill8(buf, xsize, COL8_DARKGRAY, xsize - 2, 1,         xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_BLACK, xsize - 1, 0,         xsize - 1, ysize - 1);
	boxfill8(buf, xsize, COL8_VIVGRAY, 2,         2,         xsize - 3, ysize - 3);
	boxfill8(buf, xsize, COL8_DARKBLUE, 3,         3,         xsize - 4, 20       );
	boxfill8(buf, xsize, COL8_DARKGRAY, 1,         ysize - 2, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_BLACK, 0,         ysize - 1, xsize - 1, ysize - 1);
	putfonts8_asc(buf, xsize, 24, 4, COL8_WHITE, title);
	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = COL8_BLACK;
			} else if (c == '$') {
				c = COL8_DARKGRAY;
			} else if (c == 'Q') {
				c = COL8_VIVGRAY;
			} else {
				c = COL8_WHITE;
			}
			buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
		}
	}
	return;
}
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c)
{
	int x1 = x0 + sx, y1 = y0 + sy;
	boxfill8(sht->buf, sht->bxsize, COL8_DARKGRAY, x0 - 2, y0 - 3, x1 + 1, y0 - 3);
	boxfill8(sht->buf, sht->bxsize, COL8_DARKGRAY, x0 - 3, y0 - 3, x0 - 3, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, COL8_WHITE, x0 - 3, y1 + 2, x1 + 1, y1 + 2);
	boxfill8(sht->buf, sht->bxsize, COL8_WHITE, x1 + 2, y0 - 3, x1 + 2, y1 + 2);
	boxfill8(sht->buf, sht->bxsize, COL8_BLACK, x0 - 1, y0 - 2, x1 + 0, y0 - 2);
	boxfill8(sht->buf, sht->bxsize, COL8_BLACK, x0 - 2, y0 - 2, x0 - 2, y1 + 0);
	boxfill8(sht->buf, sht->bxsize, COL8_VIVGRAY, x0 - 2, y1 + 1, x1 + 0, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, COL8_VIVGRAY, x1 + 1, y0 - 2, x1 + 1, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, c,           x0 - 1, y0 - 1, x1 + 0, y1 + 0);
	return;
}
