		ORG		0xc200		; this program should be contained at 0xc200

		MOV		AL,0x13
		MOV		AH,0x00
		INT		0x10		; video mode setting, reference to p60
fin:
		HLT
		JMP 	fin
