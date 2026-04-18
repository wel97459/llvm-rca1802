; RUN: llvm-mc -triple=rca1802-unknown-unknown -mcpu=rca1802 -show-encoding < %s | FileCheck %s

; --- Register nibble instructions ---
; CHECK: glo r0 ; encoding: [0x80]
    glo r0
; CHECK: glo r15 ; encoding: [0x8f]
    glo r15
; CHECK: ghi r7 ; encoding: [0x97]
    ghi r7
; CHECK: plo r8 ; encoding: [0xa8]
    plo r8
; CHECK: phir r9 ; encoding: [0xb9]
    phir r9
; CHECK: str r10 ; encoding: [0x5a]
    str r10
; CHECK: ldn r11 ; encoding: [0x0b]
    ldn r11
; CHECK: lda r12 ; encoding: [0x4c]
    lda r12
; CHECK: sep r13 ; encoding: [0xdd]
    sep r13
; CHECK: sex r14 ; encoding: [0xee]
    sex r14
; CHECK: inc r1 ; encoding: [0x11]
    inc r1
; CHECK: dec r2 ; encoding: [0x22]
    dec r2

; --- Immediate instructions ---
; CHECK: ldi 255 ; encoding: [0xf8,0xff]
    ldi 255
; CHECK: adi 1 ; encoding: [0xfc,0x01]
    adi 1
; CHECK: xri 128 ; encoding: [0xfb,0x80]
    xri 128

; --- Branch instructions ---
; CHECK: br 0 ; encoding: [0x30,0x00]
    br 0
; CHECK: bz 255 ; encoding: [0x32,0xff]
    bz 255
; CHECK: lbr 4660 ; encoding: [0xc0,0x12,0x34]
    lbr 0x1234
; CHECK: lbz 0 ; encoding: [0xca,0x00,0x00]
    lbz 0x0000

; --- Fixed instructions ---
; CHECK: ret ; encoding: [0x70]
    ret
; CHECK: idl ; encoding: [0x00]
    idl
; CHECK: nop ; encoding: [0xc4]
    nop
