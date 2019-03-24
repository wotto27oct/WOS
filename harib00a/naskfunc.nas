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
		GLOBAL	_load_gdtr, _load_idtr
		GLOBAL  _asm_inthandler20, _asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
		GLOBAL	_load_cr0, _store_cr0
		GLOBAL	_memtest_sub
		EXTERN	_inthandler20, _inthandler21, _inthandler27, _inthandler2c

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

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:
		MOV		AX,[ESP+4]
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

_asm_inthandler20:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler20
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
		
_asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_load_cr0:		; int load_cr0(void);
		MOV		EAX,CR0
		RET

_store_cr0:		; void store_cr0(int cr0);
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

_memtest_sub:	; unsigned int memtest_sub(unsigned int start, unsigned int end)
		PUSH	EDI
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55
		MOV		EDI,0x55aa55aa
		MOV		EAX,[ESP+12+4]		; i = start
mts_loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc			; p = i + 0xffc
		MOV		EDX,[EBX]
		MOV		[EBX],ESI
		XOR		DWORD [EBX],0xffffffff
		CMP		EDI,[EBX]
		JNE		mts_fin
		XOR		DWORD [EBX],0xffffffff
		CMP		ESI,[EBX]
		JNE		mts_fin
		MOV		[EBX],EDX
		ADD		EAX,0x1000
		CMP		EAX,[ESP+12+8]
		JNE		mts_loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
mts_fin:
		MOV		[EBX],EDX
		POP		EBX
		POP		ESI
		POP		EDI
		RET
