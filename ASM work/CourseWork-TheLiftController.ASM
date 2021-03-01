;----- CONTROLLING THE LIFT ----------------------------------------------------------
	JMP	Start
;=====================================================================================
	DB	92		; Set hardware timers interrupt to jump to A3
	DB	80		; Set keyboard input interrupt to jump to 8A
	DB	"UP "		; Store "UP "
	DB	"TOP "		; Store "TOP "
	DB	"DOWN "		; Store "DOWN "
	DB	"BOTTOM"	; Store "BOTTOM"
;=====================================================================================
Start:
	CLO			; Close all currently open windows
	OUT	07		; Display keyboard window
	OUT	06		; Display lift window
	IN	06		; Input current lift state into AL
	CMP	AL, 08		; If the lift is at the bottom
	JZ	CheckBottom	; Jump to the CheckBottom loop, to print BOTTOM
	JNZ	CheckTop	; Otherwise jump to the CheckTop loop to print TOP
;=====================================================================================
Loop:

	IN	06		; Input current lift state into AL
	AND	AL, 26		; Only check bits for the up button, top sensor and down motor
	CMP	AL, 20		; If the up button is not the only thing active
	JNZ	DontGoUp	; Then do not start the up motor

	MOV	BL, 04		; Set BL to point to "U"
	CALL	93		; Call the printing subroutine
	STI			; Set the interrupt flag on

	IN	06		; Copy current lift output to AL
	AND	AL, DD		; Turn off the Up button and the Down motor
	OR	AL, 01		; And turn on the Up motor
	OUT	06		; Output changes to the lift

CheckTop:
	IN	06		; Copy current lift output to AL
	AND	AL, 04		; If the lift has not reached the top
	JZ	CheckTop	; Loop until the lift reaches the top
	IN	06		; Copy current lift output to AL
	AND	AL, FE		; Turn off the Up motor
	OUT	06		; Output changes to the lift

	MOV	BL, 07		; Set BL to point to "T"
	CALL	93		; Call the printing subroutine
	STI			; Set the interrupt flag on
;=====================================================================================
DontGoUp:

	IN	06		; Input current lift state into AL
	AND 	AL, 19		; Only check bits for the down button, bottom sensor and up motor
	CMP	AL, 10		; If the down button is not the only thing active
	JNZ	DontGoDown	; Then do not activate the down motor
Down:
	MOV	BL, 0B		; Set BL to point to "D"
	CALL	93		; Call the printing subroutine

	IN	06		; Copy current lift output to AL
	AND	AL, EE		; Turn off the Down button and the Up motor
	OR	AL, 02		; And turn on the Down motor
	OUT	06		; Output changes to the lift

CheckBottom:
	IN	06		; Copy current lift output to AL
	AND	AL, 08		; If the lift is not at the bottom
	JZ	CheckBottom	; Loop until the lift reaches the bottom
	IN	06		; Copy current lift output to AL
	AND	AL, FD		; Turn off the Down motor
	OUT	06		; Output changes to the lift

	MOV	BL, 10		; Set BL to pint to "B"
	CALL	93		; Call the printing subroutine

DontGoDown:
	JMP	Loop		; Jump back to check if any buttons have been pressed
;=====================================================================================	
	CLI			; Clear the interrupt flag, to prevent interrupts
	IN	07		; Input key pressed on keyboard, into AL
	CMP	AL, 0D		; If the key pressed was not carraige return
	JNZ	Skip		; Then skip to the end of the interrupt handler
	POP	AL		; Clear previous instruction pointer from the stack
	AND	AL, 00		; Set AL to clear the lift state
	OUT	06		; Clear the lift state, to switch off all motors and buttons
	JMP	Down		; Jump to code to switch down motor on
Skip:
	STI			; Set interrupt flag
	IRET			; Restore the instruction pointer from the stack
;=====================================================================================	
	ORG	93
	CLI			; Clear the interrupt flag
	MOV	CL, 06		; Set CL to cout six characters
	MOV	DL, C0		; Set DL to point to the first space in the VDU
Print:
	MOV	AL, [BL]	; Move the character from address BL, into AL
	MOV	[DL], AL	; Move the character from AL, into the VDU
	CMP	AL, 20		; If the character was a space
	JZ	Clear		; Then do not increment BL to point to the next character
	INC	BL		; Otherwise increment BL to point to the next character
Clear:
	INC	DL		; Increment DL to point to the next space in the VDU
	DEC	CL		; Decrement CL to count down how many characters are left for printing
	JNZ	Print		; If there are still characters to print then loop back until they are all printed
	RET			; Restore the instruction pointer from the stack
;=====================================================================================	
	END
;-------END OF PROGRAM----------------------------------------------------------------