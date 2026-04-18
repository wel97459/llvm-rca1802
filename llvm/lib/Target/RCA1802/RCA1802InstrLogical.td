//===-- RCA1802InstrLogical.td - Describe RCA1802 Logical Instrs --*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

class RCA1802LogicalInstr : Instruction {
  let Namespace = "RCA1802";
  dag OutOperandList = (outs);
  dag InOperandList = (ins);
  let isPseudo = true;
  let isCodeGenOnly = true;
}

//===----------------------------------------------------------------------===//
// 8-bit Arithmetic/Logic Patterns
//===----------------------------------------------------------------------===//

// Add i8
def : Pat<(add GPR8:$l, GPR8:$r), (ADD)>;
def : Pat<(add GPR8:$l, imm8:$r), (ADI imm8:$r)>;

// Sub i8
def : Pat<(sub GPR8:$l, GPR8:$r), (SUB)>;
def : Pat<(sub GPR8:$l, imm8:$r), (SUI imm8:$r)>;

// And i8
def : Pat<(and GPR8:$l, GPR8:$r), (AND)>;
def : Pat<(and GPR8:$l, imm8:$r), (ANI imm8:$r)>;

// Or i8
def : Pat<(or GPR8:$l, GPR8:$r), (OR)>;
def : Pat<(or GPR8:$l, imm8:$r), (ORI imm8:$r)>;

// Xor i8
def : Pat<(xor GPR8:$l, GPR8:$r), (XOR)>;
def : Pat<(xor GPR8:$l, imm8:$r), (XRI imm8:$r)>;

//===----------------------------------------------------------------------===//
// 16-bit Register Operations
//===----------------------------------------------------------------------===//

def : Pat<(add GPR16:$r, 1), (INC GPR16:$r)>;
def : Pat<(add GPR16:$r, -1), (DEC GPR16:$r)>;

//===----------------------------------------------------------------------===//
// Load/Store Patterns
//===----------------------------------------------------------------------===//

// Load D from memory via register pointer R(N)
def : Pat<(load GPR16:$ptr), (LDN GPR16:$ptr)>;

// Store D to memory via register pointer R(N)
def : Pat<(store GPR8:$src, GPR16:$ptr), (STR GPR16:$ptr)>;

// Load D Immediate
def : Pat<(i8 imm:$imm), (LDI imm:$imm)>;

//===----------------------------------------------------------------------===//
// Register Moves (using GLO/GHI/PLO/PHI)
//===----------------------------------------------------------------------===//

// Move Low byte of R(N) to D
def : Pat<(extract_subreg GPR16:$r, sublo), (GLO GPR16:$r)>;
// Move High byte of R(N) to D
def : Pat<(extract_subreg GPR16:$r, subhi), (GHI GPR16:$r)>;

// Move D to Low byte of R(N)
// Note: In reality, we need to handle the tied register or side effects
// For now, simple patterns for instruction selection:
def : Pat<(insert_subreg GPR16:$r, GPR8:$val, sublo), (PLO GPR16:$r)>; // Simplified
def : Pat<(insert_subreg GPR16:$r, GPR8:$val, subhi), (PHI GPR16:$r)>; // Simplified
