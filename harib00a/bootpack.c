void io_hlt(void);
void write_mem8(int addr, int data); // assign 1-byte data to [addr]

void HariMain(void)
{
	int i;

	for (i = 0xa0000; i <= 0xaffff; i++) {
		write_mem8(i, 15);	// MOV BYTE [i],15
	}

	for(;;) {
		io_hlt();
	}
}
