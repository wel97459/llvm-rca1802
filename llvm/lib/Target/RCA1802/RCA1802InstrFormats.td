//===-- RCA1802InstrFormats.td - RCA1802 Instruction Formats -------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Operand types
def imm8 : Operand<i8>;
def addr8 : Operand<i8>;
def addr16 : Operand<i16>;

// Register Operands
def reg_n : Operand<i8> {
  let PrintMethod = "printRegN"; // Custom printer for R0-RF
}

def HasRCA1802 : Predicate<"Subtarget->hasRCA1802()">,
                 AssemblerPredicate<(all_of FeatureRCA1802), "RCA1802"> {
  let PredicateName = "FeatureRCA1802";
}

class Inst1802<dag outs, dag ins, string asmstr> : Instruction {
  field bits<32> Inst;
  let Namespace = "RCA1802";
  let OutOperandList = outs;
  let InOperandList = ins;
  let AsmString = asmstr;
  let DecoderNamespace = "RCA1802";

  // Default properties
  let mayLoad = false;
  let mayStore = false;
  let hasSideEffects = false;
}

// Instruction with a register nibble (e.g., LDN, STR, GLO, PHI)
// Encoding: [ Opcode(4 bits) | Register(4 bits) ]
class InstReg<bits<4> op, string mnemonic, RegisterClass rc>
  : Inst1802<(outs), (ins rc:$r), mnemonic # " $r"> {
  let Size = 1;
  bits<4> r;
  let Inst{7-4} = op;
  let Inst{3-0} = r;
}

// Fixed 1-byte instruction (e.g., LDX, ADD, RET)
class InstFixed<bits<8> op, string asmstr> : Inst1802<(outs), (ins), asmstr> {
  let Size = 1;
  let Inst{7-0} = op;
}

// Instruction with 8-bit immediate (e.g., LDI, ADI)
class InstImm8<bits<8> op, string mnemonic>
  : Inst1802<(outs), (ins imm8:$imm), mnemonic # " $imm"> {
  let Size = 2;
  bits<8> imm;
  let Inst{15-8} = op;
  let Inst{7-0} = imm;
}

// Short Branch (e.g., BR, BZ) - 8-bit address (within page)
class InstBranch8<bits<8> op, string mnemonic>
  : Inst1802<(outs), (ins addr8:$addr), mnemonic # " $addr"> {
  let Size = 2;
  let isBranch = true;
  let isTerminator = true;
  bits<8> addr;
  let Inst{15-8} = op;
  let Inst{7-0} = addr;
}

// Long Branch (e.g., LBR, LBZ) - 16-bit address
class InstBranch16<bits<8> op, string mnemonic>
  : Inst1802<(outs), (ins addr16:$addr), mnemonic # " $addr"> {
  let Size = 3;
  let isBranch = true;
  let isTerminator = true;
  bits<16> addr;
  let Inst{23-16} = op;
  let Inst{15-0} = addr;
}
