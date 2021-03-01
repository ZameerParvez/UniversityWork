; ===== TASK9: DOUBLING ================

	MOV BL,1	; Initial value stored in BL.

Rep:			; Jump back to this label.
	ADD BL,BL	; Double value in BL.
	JMP Rep		; Jump back to Rep.

	END		; Program Ends.

; ===== Program Ends ===============================