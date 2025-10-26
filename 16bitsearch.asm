ASSUME DS:DATA, CS:CODE

DATA SEGMENT
    ARR     DW 10 DUP(?)                           ; Array for 10 16-bit numbers
    COUNT   DW ?                                    ; Number of elements
    KEY     DW ?                                    ; Search key
    POS     DW ?                                    ; Position where found
    MSG1    DB 0AH,0DH,"ENTER NUMBER OF ELEMENTS (1-9): $"
    MSG2    DB 0AH,0DH,"ENTER HEXADECIMAL NUMBER (4 DIGITS): $"
    MSG3    DB 0AH,0DH,"ENTER SEARCH KEY (4 DIGITS): $"
    MSG4    DB 0AH,0DH,"ELEMENT FOUND AT POSITION: $"
    MSG5    DB 0AH,0DH,"ELEMENT NOT FOUND$"
    SPACE   DB " $"
    NEWLINE DB 0AH,0DH,"$"
DATA ENDS

CODE SEGMENT
START:
    ; ===== INITIALIZE DATA SEGMENT =====
    MOV AX, DATA
    MOV DS, AX
    
    ; ===== GET NUMBER OF ELEMENTS =====
    LEA DX, MSG1
    MOV AH, 09H
    INT 21H
    
    MOV AH, 01H            ; Read single digit
    INT 21H
    SUB AL, 30H            ; Convert ASCII to number
    MOV BL, AL
    XOR BH, BH             ; Clear BH
    MOV COUNT, BX          ; Store count
    
    ; ===== INPUT ARRAY ELEMENTS =====
    MOV CX, COUNT          ; Loop counter
    LEA SI, ARR            ; Point to array start
    
INPUT_LOOP:
    PUSH CX                ; Save loop counter
    
    LEA DX, MSG2
    MOV AH, 09H
    INT 21H
    
    CALL READ_HEX          ; Read 16-bit hex number into BX
    MOV [SI], BX           ; Store in array
    ADD SI, 2              ; Move to next word
    
    POP CX                 ; Restore loop counter
    LOOP INPUT_LOOP
    
    ; ===== INPUT SEARCH KEY =====
    LEA DX, MSG3
    MOV AH, 09H
    INT 21H
    
    CALL READ_HEX          ; Read search key into BX
    MOV KEY, BX            ; Store search key
    
    ; ===== LINEAR SEARCH ALGORITHM =====
    MOV CX, COUNT          ; Number of elements to search
    LEA SI, ARR            ; Point to array start
    MOV AX, KEY            ; Load search key
    MOV BX, 1              ; Position counter (1-based)
    
SEARCH_LOOP:
    CMP AX, [SI]           ; Compare key with current element
    JE FOUND               ; If equal, element found
    
    ADD SI, 2              ; Move to next element
    INC BX                 ; Increment position counter
    LOOP SEARCH_LOOP       ; Continue searching
    
    ; ===== ELEMENT NOT FOUND =====
    LEA DX, MSG5
    MOV AH, 09H
    INT 21H
    JMP EXIT_PROG
    
    ; ===== ELEMENT FOUND =====
FOUND:
    MOV POS, BX            ; Store position
    
    LEA DX, MSG4
    MOV AH, 09H
    INT 21H
    
    ; Display position (convert to ASCII)
    MOV AX, POS
    ADD AL, 30H            ; Convert to ASCII (works for 1-9)
    MOV DL, AL
    MOV AH, 02H
    INT 21H
    
    ; ===== EXIT PROGRAM =====
EXIT_PROG:
    MOV AH, 4CH
    INT 21H

; ========================================
; READ_HEX: Reads 4 hex digits from keyboard into BX
; ========================================
READ_HEX PROC
    PUSH CX
    PUSH AX
    
    MOV BX, 0              ; Initialize result
    MOV CX, 4              ; 4 hex digits
    
READ_DIGIT:
    MOV AH, 01H            ; Read character
    INT 21H
    
    ; Convert ASCII to hex value
    CMP AL, '9'
    JBE NUMERIC            ; If <= '9', it's 0-9
    
    ; It's A-F (convert uppercase)
    CMP AL, 'F'
    JBE UPPERCASE
    
    ; It's a-f (convert lowercase)
    SUB AL, 20H            ; Convert to uppercase
    
UPPERCASE:
    SUB AL, 37H            ; Convert 'A'-'F' to 10-15
    JMP SHIFT_IN
    
NUMERIC:
    SUB AL, 30H            ; Convert '0'-'9' to 0-9
    
SHIFT_IN:
    SHL BX, 4              ; Shift left by 4 bits
    OR BL, AL              ; Add new nibble
    
    LOOP READ_DIGIT
    
    POP AX
    POP CX
    RET
READ_HEX ENDP

CODE ENDS
END START
