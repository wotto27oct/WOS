#include "bootpack.h"

// initialize FIFO buffa
void fifo32_init(struct FIFO32 *fifo, int size, int*buf, struct TASK *task)
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size;
	fifo->flags = 0;
	fifo->p = 0; // write
	fifo->q = 0; // read
	fifo->task = task; // the task to rerun if data is put
}

// put data into FIFO
int fifo32_put(struct FIFO32 *fifo, int data)
{
	if (fifo->free == 0) {
		// no storage
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size) fifo->p = 0;
	fifo->free--;
	if (fifo->task != 0) {
		if (fifo->task->flags != 2) {
			task_run(fifo->task);
		}
	}
	return 0;
}

// get data from FIFO
int fifo32_get(struct FIFO32 *fifo)
{
	int data;
	if (fifo->free == fifo->size) {
		// buffa is empty
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size) {
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

// get data size of FIFO
int fifo32_status(struct FIFO32 *fifo)
{
	return fifo->size - fifo->free;
}
