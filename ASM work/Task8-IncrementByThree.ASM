; ===== TASK9: INCREMENT BY THREE ================

	MOV BL,0	; Initial value stored in BL.

Rep:			; Jump back to this label.
	ADD BL,3	; Increment BL by 3.
	JMP Rep		; Jump back to Rep.

	END		; Program Ends.

; ===== Program Ends ===============================