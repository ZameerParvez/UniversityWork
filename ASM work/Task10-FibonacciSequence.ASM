; ===== TASK10: FIBONACCI SEQUENCE ================

	MOV BL,1	; Initialise BL as ONE.
LOOP:
	MOV [BF],BL	; Copy value of BL into RAM.
	ADD BL,AL	; Add BL to AL.
	MOV AL,[BF]	; Copy the value stored in RAM into AL.
	JO END		; If the Overflow flag is triggered jump to the end.
	JMP LOOP	; Loop until an overflow occurs.
END:	
	END		; End program.

; ===== Program Ends ===============================