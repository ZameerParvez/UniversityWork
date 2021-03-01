; ===== TASK16: TRAFFIC LIGHTS STORED SEQUENCE=============================
	JMP START
	
			; (R = Red) (Y = yellow) (G = green) 
	DB 90		; Left light R, Right light R
	DB 98		; Left light R, Right light R+Y
	DB 84		; Left light R, Right light G
	DB 88		; Left light R, Right light Y 
	DB D0		; Left light R+Y, Right light R	
	DB 30		; Left light G, Right light R
	DB 50		; Left light Y, Right light R
	DB 04		; Short wait
	DB 08		; Medium wait
	DB 0C		; Long wait
Start:
			; (R = Red) (Y = yellow) (G = green) 

	MOV AL,[02]	; Left light R, Right light R
	OUT 01		; Output to traffic lights
	MOV BL,[09]	; Set counter to short wait
	CALL 5E		; Call procedure at 5E to delay light change

	MOV AL,[03]	; Left light R, Right light R+Y
	OUT 01
	MOV BL,[0A]	; Set counter to medium wait
	CALL 5E		; Call procedure at 5E to delay light change
	
	MOV AL,[04]	; Left light R, Right light G
	OUT 01
	MOV BL,[0B]	; Set counter to long wait
	CALL 5E		; Call procedure at 5E to delay light change

	MOV AL,[05]	; Left light R, Right light Y 
	OUT 01
	MOV BL,[0A]	; Set counter to medium wait
	CALL 5E		; Call procedure at 5E to delay light change

	MOV AL,[02]	; Left light R, Right light R
	OUT 01
	MOV BL,[09]	; Set counter to short wait
	CALL 5E		; Call procedure at 5E to delay light change

	MOV AL,[06]	; Left light R+Y, Right light R	
	OUT 01
	MOV BL,[0A]	; Set counter to medium wait
	CALL 5E		; Call procedure at 5E to delay light change

	MOV AL,[07]	; Left light G, Right light R
	OUT 01
	MOV BL,[0B]	; Set counter to long wait
	CALL 5E		; Call procedure at 5E to delay light change

	MOV AL,[08]	; Left light Y, Right light R
	OUT 01
	MOV BL,[0A]	; Set counter to medium wait
	CALL 5E		; Call procedure at 5E to delay light change

	JMP Start	; Jump back to the start.


	ORG 5E		; Set the origin of the procedure's code to address 30.
	PUSHF		; Push flags to stack.
COUNT:			; Tag for a counter loop, counts down from value in BL to keep lights on for longer.
	DEC BL
	JNZ COUNT	; If BL is not zero then go to count.
	POPF		; Restore flags from stack.
	RET		; Return control back to main program, subroutine ends.

	END		; Program ends.
	
; ===== Program Ends ==========================================