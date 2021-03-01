;==== TASK13: IO BATCH IO ===================================================

	CLO		; Close unwanted windows.
	MOV BL,30	; Initialise BL to unused RAM to start storing text.
	MOV CL,C0	; Initialise CL to first RAM address used by the VDU.
REP:
	IN 00		; Wait for key press - Store it in AL.
	MOV [BL],AL	; Move entered value into the first RAM used by the VDU.
	INC BL		; Increment BL to point at the next address in the RAM to store the character.
	CMP AL,0D	; Was it the Enter key? (ASCII 0D).
	JNZ REP		; No - jump back.  Yes - continue to output.
	MOV BL,30	; Reset value of BL.
DISPLAY:
	MOV DL,[BL]	; Move character stored at address BL in RAM to DL.
	MOV [CL],DL	; Move character stored in DL into first VDU RAM address.
	INC BL		; Increment BL to point at the next character in the RAM.
	INC CL		; Increment CL to point at the next address in the VDU.
	CMP DL,0D	; Check if Enter Key is stored in DL.
	JNZ DISPLAY	; If not jump back, if it is then end
END

;===== Program End ==============================================