#include "bootpack.h"

struct FIFO32 *mousefifo;
int mousedata0;

void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec)
{
	mousefifo = fifo;
	mousedata0 = data0;
	/* マウス有効 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	mdec->phase = 0;
	return; /* うまくいくとACK(0xfa)が送信されてくる */
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	if (mdec->phase == 0) {
		// wait for 0xfa
		if (dat == 0xfa) {
			mdec->phase = 1;
		}
		return 0;
	} else if (mdec->phase == 1) {
		// wait for first-byte
		if ((dat & 0xc8) == 0x08){
			// it's correct first-byte
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	} else if (mdec->phase == 2) {
		// wait for second-byte
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	} else if (mdec->phase == 3) {
		// wait for third-byte
		mdec->buf[2] = dat;
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if ((mdec->buf[0] & 0x10) != 0) {
			mdec->x |= 0xffffff00;
		}
		if ((mdec->buf[0] & 0x20) != 0) {
			mdec->y |= 0xffffff00;
		}
		mdec->y = - mdec->y;
		return 1;
	}
	return -1;
}

// interrupt from PS/2 mouse
void inthandler2c(int *esp)
{
	int data;
	io_out8(PIC1_OCW2, 0x64);	// report PIC1 that IRQ-12 is successfully accepted
	io_out8(PIC0_OCW2, 0x62);	// report PIC0 that IRQ-02 is successfully accepted
	data = io_in8(PORT_KEYDAT);
	fifo32_put(mousefifo, data + mousedata0);
	return;
}
