[BITS 32]
	MOV		AL,'A'
	CALL	2*8:0xc53
fin:
	HLT
	JMP fin
