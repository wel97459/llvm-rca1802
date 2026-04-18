; RUN: llvm-mc -filetype=obj -triple=rca1802-unknown-unknown -mcpu=rca1802 < %s | llvm-readobj -h - | FileCheck %s

; CHECK: Class: 32-bit
; CHECK: DataEncoding: BigEndian
; CHECK: Machine: EM_RCA1802 (0x70A)

    nop
