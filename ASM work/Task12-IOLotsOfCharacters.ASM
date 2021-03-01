;==== TASK12: IO LOTS OF CHARACTERS ======================================

	CLO		; Close unwanted windows.
	MOV BL,C0
REP:
	IN 00		; Wait for key press - Store it in AL.
	MOV [BL],AL	; Move entered value into the first RAM used by the VDU.
	INC BL		; Increment BL to point at the next address in the VDU.
	CMP AL,0D	; Was it the Enter key? (ASCII 0D).
	JNZ REP		; No - jump back.  Yes - end.	
END

;===== Program End ==============================================