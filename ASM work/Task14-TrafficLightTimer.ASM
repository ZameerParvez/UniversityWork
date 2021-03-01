; ===== TASK14: TRAFFIC LIGHT TIMER =============================
	
	CLO		; Close unwanted windows.
Start:
			; (R = Red) (Y = yellow) (G = green) 

	MOV AL,90	; Left light R, Right light R
	OUT 01		; Output to traffic lights
	MOV BL,10	; Set counter to 10
	CALL 53		; Call procedure at 53 to delay light change

	MOV AL,98	; Left light R, Right light R+Y
	OUT 01
	MOV BL,5	; Set counter to 5
	CALL 53		; Call procedure at 53 to delay light change
	
	MOV AL,84	; Left light R, Right light G
	OUT 01
	MOV BL,10	; Set counter to 10
	CALL 53		; Call procedure at 53 to delay light change

	MOV AL,88	; Left light R, Right light Y 
	OUT 01
	MOV BL,5	; Set counter to 5
	CALL 53		; Call procedure at 53 to delay light change

	MOV AL,90	; Left light R, Right light R
	OUT 01
	MOV BL,10	; Set counter to 10
	CALL 53		; Call procedure at 53 to delay light change

	MOV AL,D0	; Left light R+Y, Right light R	
	OUT 01
	MOV BL,5	; Set counter to 5
	CALL 53		; Call procedure at 53 to delay light change

	MOV AL,30	; Left light G, Right light R
	OUT 01
	MOV BL,10	; Set counter to 10
	CALL 53		; Call procedure at 53 to delay light change

	MOV AL,50	; Left light Y, Right light R
	OUT 01
	MOV BL,5	; Set counter to 5
	CALL 53		; Call procedure at 53 to delay light change

	JMP Start	; Jump back to the start.


	ORG 53		; Set the origin of the procedure's code to address 30.
	PUSHF		; Push flags to stack.
COUNT:			; Tag for a counter loop, counts down from value in BL to keep lights on for longer.
	DEC BL
	JNZ COUNT	; If BL is not zero then go to count.
	POPF		; Restore flags from stack.
	RET		; Return control back to main program, subroutine ends.

	END		; Program ends.
	
; ===== Program Ends ==========================================