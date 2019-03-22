; naskfunc

[FORMAT "WCOFF"]		; mode for making object file
[BITS 32]				; machine language for 32bit

; information for object file
[FILE "naskfunc.nas"]	; source file name info
		GLOBAL	_io_hlt	; function name contains in this program

; function entity

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET
