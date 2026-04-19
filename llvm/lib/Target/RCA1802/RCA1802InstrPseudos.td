//===-- RCA1802InstrPseudos.td - RCA1802 Pseudo instructions -------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

let isPseudo = 1, isCodeGenOnly = 1 in {

def ADJCALLSTACKDOWN : Inst1802<(outs), (ins i16imm:$amt1, i16imm:$amt2),
                                "#ADJCALLSTACKDOWN $amt1 $amt2">;
def ADJCALLSTACKUP   : Inst1802<(outs), (ins i16imm:$amt1, i16imm:$amt2),
                                "#ADJCALLSTACKUP $amt1 $amt2">;

// Copy register
def COPY_1802 : Inst1802<(outs GPR16:$dst), (ins GPR16:$src), "copy $dst, $src">;

let isCall = 1, Defs = [R2, R3, R4, R5, R6] in {
  def CALL_PSEUDO : Inst1802<(outs), (ins addr16:$addr), "sep r4 ; dw $addr">;
}

let isReturn = 1, isTerminator = 1, isBarrier = 1 in {
  def RET_PSEUDO : Inst1802<(outs), (ins), "sep r5">;
}

} // isPseudo, isCodeGenOnly
