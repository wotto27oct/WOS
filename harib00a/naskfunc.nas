; naskfunc

[FORMAT "WCOFF"]		; mode for making object file
[INSTRSET "i486p"]
[BITS 32]				; machine language for 32bit

; information for object file
[FILE "naskfunc.nas"]	; source file name info

		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _oi_out32
		GLOBAL	_io_load_eflags, _io_store_eflags

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_io_cli:
		CLI
		RET

_io_sti:
		STI
		RET

_io_stihlt:
		STI
		HLT
		RET

_io_in8:		;int io_in8(int port);
		MOV		EDX,[ESP+4]
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:
		MOV		EDX,[ESP+4]
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:
		MOV		EDX,[ESP+4]
		IN		EAX,DX
		RET

_io_out8:		; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		AL,[ESP+8];
		OUT		DX,AL
		RET

_io_out16:		; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		EAX,[ESP+8];
		OUT		DX,AX
		RET

_io_out32:		; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]
		MOV		EAX,[ESP+8];
		OUT		DX,EAX
		RET

_io_load_eflags:
		PUSHFD		; = PUSH EFLAGS
		POP		EAX
		RET

_io_store_eflags:
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; = POP EFLAGS
		RET
