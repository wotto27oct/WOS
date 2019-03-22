; naskfunc

[FORMAT "WCOFF"]		; mode for making object file
[INSTRSET "i486p"]
[BITS 32]				; machine language for 32bit

; information for object file
[FILE "naskfunc.nas"]	; source file name info

		GLOBAL	_io_hlt,_write_mem8
; function entity

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_write_mem8:	; void write_mem8(int addr, int data);
		MOV		ECX,[ESP+4]		; addr is in [ESP+4]
		MOV		AL,[ESP+8]		; because ESP is stack pointer.
		MOV		[ECX],AL
		RET
