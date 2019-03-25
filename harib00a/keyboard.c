#include "bootpack.h"

struct FIFO32 *keyfifo;
int keydata0;


void wait_KBC_sendready(void)
{
	/* �L�[�{�[�h�R���g���[�����f�[�^���M�\�ɂȂ�̂�҂� */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(struct FIFO32 *fifo, int data0)
{
	keyfifo = fifo;
	keydata0 = data0;
	/* �L�[�{�[�h�R���g���[���̏����� */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

// interrupt from PS/2 Keyboard
void inthandler21(int *esp)
{
	int data;
	io_out8(PIC0_OCW2, 0x61);	// report PIC that IRQ-01 is successfully accepted
	data = io_in8(PORT_KEYDAT);

	fifo32_put(keyfifo, data + keydata0);
	return;
}
