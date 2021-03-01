;==== TASK15: PASSING PARAMETERS ===================================

	CLO
	MOV AL,15	; Initialise AL as the number you want to double.
	CALL 30		; Call doubling procedure starting at address 30.

	ORG 30		; Doubling procedure starting at address 30.
	PUSHF		; Push flags to stack.
	PUSH BL		; Push value in BL to stack.
	PUSH AL		; Push value in AL to stack.
	POP BL		; Move value at the top of the stack into BL, same as AL.
	ADD AL,BL	; Add value of AL and BL, which are the same.
	POP BL		; Restore the value of BL before the procedure was called.
	POPF		; Restore the state of the flags before the procedure was called.
	RET

	END

;==== Program Ends ===================================================