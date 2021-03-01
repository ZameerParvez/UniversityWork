;===== TASK17: TEXT IO ===============================================

	JMP START

	DB 71		; Define the starting position of where the characters are stored

START:
	CLO
	CALL 0B
	CALL 25
	CALL 4D
	HALT		; End Program


	ORG 0B		; Start of code to take input from keyboard until "Enter" is pressed
	PUSH AL		; Store general purpose registers values and status registers value on the stack
	PUSH BL
	PUSHF
	MOV BL,[2]	; Store the value at 2, in RAM, in BL, this value is the starting position of the input characters
READ:
	IN 00		; Take input from keyboard, the input is stored in AL
	MOV [BL],AL	; Move the value stored in AL
	INC BL		; Increment BL so that it points to the next character
	CMP AL,0D	; If the inputted character is not "Enter"
	JNZ READ	; Then jump back to READ loop to input more characters
	POPF		; Restore values from the stack
	POP BL
	POP AL
	RET		; Restore the Instruction Pointers value from the stack


	ORG 25		; Start of code to change uppercase letters to lowercase
	PUSHF		; Store general purpose registers values and status registers value on the stack
	PUSH AL
	PUSH BL
	MOV BL,[2]	; Store the value at 2, in RAM, in BL, this value is the starting position of the input characters
LOOP:
	MOV AL,[BL]	; Move the first letter into AL
	CMP AL,41	; If the character in AL is less than "A" 
	JS CONT		; Then jump to CONT
	CMP AL,5B	; If AL is not less than "["
	JNS CONT	; Jump to CONT
	ADD AL,20	; ADD 20 to the ascii code in AL to give the lowercase character
	MOV [BL],AL	; Replace the original character with the lower case character
CONT:
	INC BL		; Increment BL to point to the next character
	CMP AL,0D	; If AL is not equal to 0D, "Carriage Return"
	JNZ LOOP	; Then jump back to loop to until all characters are read
	POP BL		; Restore values from the stack
	POP AL
	POPF
	RET		; Restore the Instruction Pointers value from the stack


	ORG 4D		; Procedure to move characters stored in RAM into VDU space
	PUSHF		; Store general purpose registers values and status registers value on the stack
	PUSH CL
	PUSH BL
	PUSH AL
	MOV BL,[2]	; Store the value at 2, in RAM, in BL, this value is the starting position of the input characters
	MOV AL,C0	; Store C0 in AL, this is the first address th VDU uses
DISPLAY:
	MOV CL,[BL]	; Move the character stored in ram, into CL
	MOV [AL],CL	; Move the character from AL to the VDU
	INC AL		; Increment AL to point to the next VDU space
	INC BL		; Increment BL to point to the next character stored in RAM
	CMP CL,0D	; If the character put into the VDU is not 0D, "Carraige Return"
	JNZ DISPLAY	; Then jump back to DISPLAY until all characters have been moved to VDU
	POP AL		; Restore values from the stack
	POP BL
	POP CL
	POPF
	RET		; Restore the Instruction Pointers value from the stack


	END

;==== Program End ==============================================
