ASSUME DS:DATA, CS:CODE

DATA SEGMENT
    MSG  DB 0AH,0DH,"ENTER FIRST NUMBER:$"
    MSG2 DB 0AH,0DH,"ENTER SECOND NUMBER:$"
    MSG3 DB 0AH,0DH,"SUM IS: $"
DATA ENDS

CODE SEGMENT
START:
    ; ===== INITIALIZE DATA SEGMENT =====
    MOV AX, DATA
    MOV DS, AX
    
    ; ===== PROMPT FOR FIRST NUMBER =====
    LEA DX, MSG
    MOV AH, 09H        ; Display string function
    INT 21H
    
    ; ===== READ FIRST NUMBER (2 DIGITS) =====
    MOV AH, 01H        ; Read character function
    INT 21H
    MOV BH, AL         ; Store first digit in BH
    
    MOV AH, 01H        ; Read second digit
    INT 21H
    MOV BL, AL         ; Store second digit in BL
    
    ; ===== PROMPT FOR SECOND NUMBER =====
    LEA DX, MSG2
    MOV AH, 09H
    INT 21H
    
    ; ===== READ SECOND NUMBER (2 DIGITS) =====
    MOV AH, 01H
    INT 21H
    MOV CH, AL         ; Store first digit in CH
    
    MOV AH, 01H
    INT 21H
    MOV CL, AL         ; Store second digit in CL
    
    ; ===== ADD LOWER DIGITS (BL + CL) =====
    MOV AL, BL         ; Get second digit of first number
    MOV AH, 00H        ; Clear AH for addition
    ADD AL, CL         ; Add second digits
    AAA                ; ASCII Adjust After Addition
    ADD AX, 3030H      ; Convert to ASCII characters
    MOV BL, AL         ; Store units digit of result
    
    ; ===== ADD HIGHER DIGITS (BH + CH + CARRY) =====
    MOV AL, AH         ; Get carry from previous addition
    MOV AH, 00H        ; Clear AH
    ADD AL, BH         ; Add first digit of first number
    ADD AL, CH         ; Add first digit of second number
    AAA                ; ASCII Adjust After Addition
    ADD AX, 3030H      ; Convert to ASCII
    MOV BH, AL         ; Store tens digit of result
    MOV CL, AH         ; Store hundreds digit (carry) in CL
    
    ; ===== DISPLAY RESULT MESSAGE =====
    LEA DX, MSG3
    MOV AH, 09H
    INT 21H
    
    ; ===== DISPLAY RESULT (3 DIGITS) =====
    MOV DL, CL         ; Display hundreds digit
    MOV AH, 02H        ; Display character function
    INT 21H
    
    MOV DL, BH         ; Display tens digit
    MOV AH, 02H
    INT 21H
    
    MOV DL, BL         ; Display units digit
    MOV AH, 02H
    INT 21H
    
    ; ===== EXIT PROGRAM =====
    MOV AH, 4CH        ; Terminate program
    INT 21H
    
CODE ENDS
END START
