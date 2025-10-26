ASSUME DS:DATA, CS:CODE

DATA SEGMENT
    MSG  DB 0AH,0DH,"ENTER FIRST NUMBER: $"
    MSG2 DB 0AH,0DH,"ENTER SECOND NUMBER: $"
    MSG3 DB 0AH,0DH,"DIFFERENCE IS: $"
    MSG4 DB 0AH,0DH,"DIFFERENCE IS: NEGATIVE $"
DATA ENDS

CODE SEGMENT
START:
    ; ===== INITIALIZE DATA SEGMENT =====
    MOV AX, DATA
    MOV DS, AX
    
    ; ===== PROMPT AND READ FIRST NUMBER (2 DIGITS) =====
    LEA DX, MSG
    MOV AH, 09H            ; Display string
    INT 21H
    
    MOV AH, 01H            ; Read first digit
    INT 21H
    MOV BH, AL             ; Store tens digit in BH
    
    MOV AH, 01H            ; Read second digit
    INT 21H
    MOV BL, AL             ; Store units digit in BL
    
    ; ===== PROMPT AND READ SECOND NUMBER (2 DIGITS) =====
    LEA DX, MSG2
    MOV AH, 09H
    INT 21H
    
    MOV AH, 01H            ; Read first digit
    INT 21H
    MOV CH, AL             ; Store tens digit in CH
    
    MOV AH, 01H            ; Read second digit
    INT 21H
    MOV CL, AL             ; Store units digit in CL
    
    ; ===== COMPARE NUMBERS TO DETERMINE SIGN =====
    CMP BX, CX             ; Compare first number with second
    JE ZERO                ; If equal, result is 00
    JB NEGATIVE            ; If first < second, result is negative
    
    ; ===== POSITIVE SUBTRACTION (BX >= CX) =====
    ; Subtract units digits
    MOV AL, BL
    SUB AL, CL
    AAS                    ; ASCII Adjust After Subtraction
    ADD AL, 30H            ; Convert to ASCII
    MOV BL, AL             ; Store units result
    
    ; Subtract tens digits with borrow
    MOV AL, BH
    SBB AL, CH             ; Subtract with borrow
    AAS                    ; ASCII Adjust After Subtraction
    ADD AL, 30H            ; Convert to ASCII
    MOV BH, AL             ; Store tens result
    
    ; Display positive result
    LEA DX, MSG3
    MOV AH, 09H
    INT 21H
    
    MOV DL, BH             ; Display tens digit
    MOV AH, 02H
    INT 21H
    
    MOV DL, BL             ; Display units digit
    MOV AH, 02H
    INT 21H
    
    JMP EXIT_PROG
    
    ; ===== ZERO RESULT =====
ZERO:
    LEA DX, MSG3
    MOV AH, 09H
    INT 21H
    
    MOV DL, '0'            ; Display "00"
    MOV AH, 02H
    INT 21H
    MOV AH, 02H
    INT 21H
    
    JMP EXIT_PROG
    
    ; ===== NEGATIVE RESULT (CX > BX) =====
NEGATIVE:
    ; Subtract units digits (reversed)
    MOV AL, CL
    SUB AL, BL
    AAS                    ; ASCII Adjust After Subtraction
    ADD AL, 30H            ; Convert to ASCII
    MOV CL, AL             ; Store units result
    
    ; Subtract tens digits with borrow (reversed)
    MOV AL, CH
    SBB AL, BH             ; Subtract with borrow
    AAS                    ; ASCII Adjust After Subtraction
    ADD AL, 30H            ; Convert to ASCII
    MOV CH, AL             ; Store tens result
    
    ; Display negative message and result
    LEA DX, MSG4
    MOV AH, 09H
    INT 21H
    
    MOV DL, CH             ; Display tens digit
    MOV AH, 02H
    INT 21H
    
    MOV DL, CL             ; Display units digit
    MOV AH, 02H
    INT 21H
    
    ; ===== EXIT PROGRAM =====
EXIT_PROG:
    MOV AH, 4CH            ; Terminate program
    INT 21H
    
CODE ENDS
END START
