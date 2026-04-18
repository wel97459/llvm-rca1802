; RUN: llvm-mc -arch=rca1802 < %s | FileCheck %s

; CHECK: .text
.text
    nop
; CHECK: nop
