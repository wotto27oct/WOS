#ifndef BOOTPACK
#define BOOTPACK

// asmhead.nas
struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0

// naskfunc.nas
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

// graphic.c
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y);
void putfont8(char*, int, int, int, char, char*);
void putfont8_sec(char*, int, int, int, char, unsigned char*);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
		int pysize, int px0, int py0, char *buf, int bxsize);

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

// dsctbl.c
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a

// int.c
void init_pit(void);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
#endif BOOTPACK