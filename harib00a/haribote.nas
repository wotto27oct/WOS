; related to BOOT_INFO
CYLS	EQU		0x0ff0			; already set in ipl10.nas
LEDS	EQU		0x0ff1			; keyboard LED information
VMODE	EQU		0x0ff2			; color information
SCRNX	EQU		0x0ff4			; screen x
SCRNY	EQU		0x0ff6			; screen y
VRAM	EQU		0x0ff8			; memory location of graphic buffa
		
		ORG		0xc200			; this program should be contained at 0xc200

		MOV		AL,0x13
		MOV		AH,0x00
		INT		0x10			; video mode setting, reference to p60
		MOV		BYTE [VMODE],8	; memo graphic mode
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; state of keyboard LED
		
		MOV		AH,0x02
		INT		0x16			; keyboard BIOS
		MOV		[LEDS],AL
fin:
		HLT
		JMP 	fin
