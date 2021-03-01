;==== TASK11: IO Characters ===================================================

	CLO		; Close unwanted windows.
REP:
	IN 00		; Wait for key press - Store it in AL.
	MOV [C0],AL	; Move entered value into the first RAM used by the VDU.
	CMP AL,0D	; Was it the Enter key? (ASCII 0D).
	JNZ REP		; No - jump back.  Yes - end.

END

;==== Program End ==============================================