ASSUME DS:DATA, CS:CODE

DATA SEGMENT
    ARR     DW 10 DUP(?)                           ; Array for 10 16-bit numbers
    COUNT   DW ?                                    ; Number of elements
    MSG1    DB 0AH,0DH,"ENTER NUMBER OF ELEMENTS (1-9): $"
    MSG2    DB 0AH,0DH,"ENTER HEXADECIMAL NUMBER (4 DIGITS): $"
    MSG3    DB 0AH,0DH,"UNSORTED ARRAY: $"
    MSG4    DB 0AH,0DH,"SORTED ARRAY: $"
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
    
    ; ===== DISPLAY UNSORTED ARRAY =====
    LEA DX, MSG3
    MOV AH, 09H
    INT 21H
    
    CALL DISPLAY_ARRAY
    
    ; ===== BUBBLE SORT ALGORITHM =====
    MOV CX, COUNT          ; Outer loop counter
    DEC CX                 ; Need n-1 passes
    
OUTER_LOOP:
    MOV BX, CX             ; Save outer loop counter
    MOV CX, COUNT          ; Inner loop counter
    DEC CX                 ; Number of comparisons
    LEA SI, ARR            ; Point to start of array
    
INNER_LOOP:
    MOV AX, [SI]           ; Load current element
    MOV DX, [SI+2]         ; Load next element
    
    CMP AX, DX             ; Compare current with next
    JBE NO_SWAP            ; If current <= next, no swap
    
    ; ===== SWAP ELEMENTS =====
    MOV [SI], DX           ; Put smaller in current position
    MOV [SI+2], AX         ; Put larger in next position
    
NO_SWAP:
    ADD SI, 2              ; Move to next element
    LOOP INNER_LOOP
    
    MOV CX, BX             ; Restore outer loop counter
    LOOP OUTER_LOOP
    
    ; ===== DISPLAY SORTED ARRAY =====
    LEA DX, MSG4
    MOV AH, 09H
    INT 21H
    
    CALL DISPLAY_ARRAY
    
    ; ===== EXIT PROGRAM =====
    MOV AH, 4CH
    INT 21H

; ========================================
; READ_HEX: Reads 4 hex digits from keyboard into BX
; ========================================
READ_HEX PROC
    PUSH CX
    MOV BX, 0              ; Initialize result
    MOV CX, 4              ; 4 hex digits
    
READ_DIGIT:
    MOV AH, 01H            ; Read character
    INT 21H
    
    ; Convert ASCII to hex value
    CMP AL, '9'
    JBE NUMERIC            ; If <= '9', it's 0-9
    
    ; It's A-F
    SUB AL, 37H            ; Convert 'A'-'F' to 10-15
    JMP SHIFT_IN
    
NUMERIC:
    SUB AL, 30H            ; Convert '0'-'9' to 0-9
    
SHIFT_IN:
    SHL BX, 4              ; Shift left by 4 bits
    OR BL, AL              ; Add new nibble
    
    LOOP READ_DIGIT
    
    POP CX
    RET
READ_HEX ENDP

; ========================================
; DISPLAY_ARRAY: Displays all array elements
; ========================================
DISPLAY_ARRAY PROC
    PUSH CX
    PUSH SI
    
    MOV CX, COUNT          ; Number of elements
    LEA SI, ARR            ; Point to array start
    
PRINT_LOOP:
    PUSH CX                ; Save counter
    
    MOV BX, [SI]           ; Load 16-bit number
    CALL PRINT_HEX         ; Print as 4 hex digits
    
    ; Print space
    LEA DX, SPACE
    MOV AH, 09H
    INT 21H
    
    ADD SI, 2              ; Move to next word
    
    POP CX                 ; Restore counter
    LOOP PRINT_LOOP
    
    ; Print newline
    LEA DX, NEWLINE
    MOV AH, 09H
    INT 21H
    
    POP SI
    POP CX
    RET
DISPLAY_ARRAY ENDP

; ========================================
; PRINT_HEX: Prints 4 hex digits from BX
; ========================================
PRINT_HEX PROC
    PUSH CX
    PUSH BX
    
    MOV CX, 4              ; 4 hex digits
    
NEXT_DIGIT:
    ROL BX, 4              ; Rotate next nibble into low 4 bits
    MOV AL, BL
    AND AL, 0FH            ; Mask to get nibble
    
    ; Convert to ASCII
    CMP AL, 09H
    JBE ADD_ZERO
    ADD AL, 37H            ; Convert 10-15 to 'A'-'F'
    JMP PRINT_IT
    
ADD_ZERO:
    ADD AL, 30H            ; Convert 0-9 to '0'-'9'
    
PRINT_IT:
    MOV DL, AL
    MOV AH, 02H            ; Display character
    INT 21H
    
    LOOP NEXT_DIGIT
    
    POP BX
    POP CX
    RET
PRINT_HEX ENDP

CODE ENDS
END START
