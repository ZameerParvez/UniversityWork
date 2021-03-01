; ===== TASK5: TRAFFIC LIGHT SEQUENCE =============================
	
	CLO		; Close unwanted windows.
START:
			; (R = Red) (Y = yellow) (G = green) 

	MOV AL,90	; Left light R, Right light R
	OUT 01		; Output to traffic lights

	MOV AL,98	; Left light R, Right light R+Y
	OUT 01
	
	MOV AL,84	; Left light R, Right light G
	OUT 01

	MOV AL,88	; Left light R, Right light Y 
	OUT 01

	MOV AL,90	; Left light R, Right light R
	OUT 01

	MOV AL,D0	; Left light R+Y, Right light R	
	OUT 01

	MOV AL,30	; Left light G, Right light R
	OUT 01

	MOV AL,50	; Left light Y, Right light R
	OUT 01

	JMP START	; Jump back to the start.
	END		; Program ends.
	
; ===== Program Ends ==========================================