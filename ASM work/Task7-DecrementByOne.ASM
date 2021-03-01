; ===== TASK7: DECREMENT BY ONE ======================================

	MOV BL,40	; Initial value stored in BL.

REP:			; Jump back to this label.
	DEC BL		; Subtract 1 from BL.
	JMP REP		; Jump back to Rep.

	END		; Program Ends.

; ===== Program Ends ===============================