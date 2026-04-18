    ; RCA 1802 Test Assembly
    ldi 0x55        ; Load 0x55 into D
    plo r7          ; Move D to R7.0
    ldi 0xAA
    phi r7          ; Move D to R7.1
    
    inc r7          ; Increment R7 (16-bit)
    
    sep r4          ; SCRT Call (Pseudo)
    dw 0x1234       ; Subroutine address
    
    ret_pseudo      ; SCRT Return (Pseudo)
