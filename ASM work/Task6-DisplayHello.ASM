; ===== TASK6: DISPLAY HELLO ==============================================================

	CLO		; Close all open windows.
	MOV AL,48	; Moves the hexcode for 'H' into AL.
	MOV [C0],AL	; Moves the contents of AL into the area of RAM used by the display.
	MOV AL,45	; Moves the hexcode for 'E' into AL.
	MOV [C1],AL	; Moves the contents of AL into the area of RAM used by the display.
	MOV AL,4C	; Moves the hexcode for 'L' into AL.
	MOV [C2],AL	; Moves the contents of AL into the area of RAM used by the display.
	MOV AL,4C	; Moves the hexcode for 'L' into AL.
	MOV [C3],AL	; Moves the contents of AL into the area of RAM used by the display.
	MOV AL,4F	; Moves the hexcode for 'O' into AL.
	MOV [C4],AL	; Moves the contents of AL into the area of RAM used by the display.
	END		; Stops the program.

; ===== Program Ends =======================================================================