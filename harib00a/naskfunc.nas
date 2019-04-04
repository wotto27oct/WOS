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
		GLOBAL	_load_tr, _farjmp
		GLOBAL	_farcall
		GLOBAL	_asm_hrb_api, _start_app
		EXTERN	_inthandler20, _inthandler21, _inthandler27, _inthandler2c
		EXTERN	_hrb_api

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
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler20
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler20
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler21
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler27
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler27
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler2c
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
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

_load_tr:
		LTR		[ESP+4]
		RET
		
_farjmp:		; void farjmp(int eip, int cs);
		JMP		FAR [ESP+4]
		RET

_farcall:		; void farcall(int eip, int cs);
		CALL	FAR [ESP+4]
		RET
	
_asm_hrb_api: ; p430
		PUSH 	DS
		PUSH	ES
		PUSHAD
		MOV		EAX,1*8
		MOV		DS,AX
		MOV		ECX,[0xfe4]
		ADD		ECX,-40
		MOV		[ECX+32],ESP
		MOV		[ECX+36],SS
		
		
		MOV		EDX,[ESP   ]
		MOV		EBX,[ESP+ 4]
		MOV		[ECX   ],EDX	; hrb_apiに渡すためコピー
		MOV		[ECX+ 4],EBX	; hrb_apiに渡すためコピー
		MOV		EDX,[ESP+ 8]
		MOV		EBX,[ESP+12]
		MOV		[ECX+ 8],EDX	; hrb_apiに渡すためコピー
		MOV		[ECX+12],EBX	; hrb_apiに渡すためコピー
		MOV		EDX,[ESP+16]
		MOV		EBX,[ESP+20]
		MOV		[ECX+16],EDX	; hrb_apiに渡すためコピー
		MOV		[ECX+20],EBX	; hrb_apiに渡すためコピー
		MOV		EDX,[ESP+24]
		MOV		EBX,[ESP+28]
		MOV		[ECX+24],EDX	; hrb_apiに渡すためコピー
		MOV		[ECX+28],EBX	; hrb_apiに渡すためコピー

		MOV		ES,AX			; 残りのセグメントレジスタもOS用にする
		MOV		SS,AX
		MOV		ESP,ECX
		STI			; やっと割り込み許可

		CALL	_hrb_api

		MOV		ECX,[ESP+32]	; アプリのESPを思い出す
		MOV		EAX,[ESP+36]	; アプリのSSを思い出す
		CLI
		MOV		SS,AX
		MOV		ESP,ECX
		POPAD
		POP		ES
		POP		DS
		IRETD		; この命令が自動でSTIしてくれる


_start_app:		; void start_app(int eip, int cs, int esp, int ds);
		PUSHAD
		MOV		EAX,[ESP+36]
		MOV		ECX,[ESP+40]
		MOV		EDX,[ESP+44]
		MOV		EBX,[ESP+48]
		MOV		[0xfe4],ESP
		CLI
		MOV		ES,BX
		MOV		SS,BX
		MOV		DS,BX
		MOV		FS,BX
		MOV		GS,BX
		MOV		ESP,EDX
		STI
		PUSH	ECX
		PUSH	EAX
		CALL	FAR [ESP]  ; call app

; app ends
		
		MOV		EAX,1*8
		CLI
		MOV		ES,AX
		MOV		SS,AX
		MOV		DS,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		ESP,[0xfe4]
		STI
		POPAD
		RET
