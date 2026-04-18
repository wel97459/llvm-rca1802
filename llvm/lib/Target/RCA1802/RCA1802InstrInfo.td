//===-- RCA1802InstrInfo.td - RCA1802 Instruction defs -------------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

let Predicates = [HasRCA1802] in {

//===----------------------------------------------------------------------===//
// Register Operations
//===----------------------------------------------------------------------===//

// Load D from Low byte of R(N)
def GLO : InstReg<0x8, "glo", GPR16> {
  let mayLoad = true;
}

// Load D from High byte of R(N)
def GHI : InstReg<0x9, "ghi", GPR16> {
  let mayLoad = true;
}

// Store D to Low byte of R(N)
def PLO : InstReg<0xA, "plo", GPR16> {
  let mayStore = true;
}

// Store D to High byte of R(N)
def PHIR : InstReg<0xB, "phir", GPR16> {
  let mayStore = true;
}
def : InstAlias<"phi $r", (PHIR GPR16:$r)>;

// Increment R(N)
def INC : InstReg<0x1, "inc", GPR16>;

// Decrement R(N)
def DEC : InstReg<0x2, "dec", GPR16>;

// Set P Designator
def SEP : InstReg<0xD, "sep", GPR16>;

// Set X Designator
def SEX : InstReg<0xE, "sex", GPR16>;

// Save T and X
def SAV : InstFixed<0x78, "sav">;

// Mark Stack
def MARK : InstFixed<0x79, "mark">;

//===----------------------------------------------------------------------===//
// Memory Operations
//===----------------------------------------------------------------------===//

// Load D via R(N) (N != 0)
def LDN : InstReg<0x0, "ldn", GPR16> {
  let mayLoad = true;
}

// Load D via R(N) and Increment
def LDA : InstReg<0x4, "lda", GPR16> {
  let mayLoad = true;
}

// Store D via R(N)
def STR : InstReg<0x5, "str", GPR16> {
  let mayStore = true;
}

// Load D via R(X)
def LDX : InstFixed<0xF0, "ldx"> {
  let mayLoad = true;
}

// Load D via R(X) and Advance
def LDXA : InstFixed<0x72, "ldxa"> {
  let mayLoad = true;
}

// Store D via R(X) and Decrement
def STXD : InstFixed<0x73, "stxd"> {
  let mayStore = true;
}

// Load D Immediate
def LDI : InstImm8<0xF8, "ldi">;

//===----------------------------------------------------------------------===//
// Arithmetic and Logic
//===----------------------------------------------------------------------===//

def ADD : InstFixed<0xF4, "add">;
def ADC : InstFixed<0x74, "adc">;
def ADI : InstImm8<0xFC, "adi">;
def ADCI : InstImm8<0x7C, "adci">;

def SUB : InstFixed<0xF5, "sub">;
def SDB : InstFixed<0x75, "sdb">;
def SUI : InstImm8<0xFD, "sui">;
def SDBI : InstImm8<0x7D, "sdbi">;

def OR : InstFixed<0xF1, "or">;
def ORI : InstImm8<0xF9, "ori">;
def AND : InstFixed<0xF2, "and">;
def ANI : InstImm8<0xFA, "ani">;
def XOR : InstFixed<0xF3, "xor">;
def XRI : InstImm8<0xFB, "xri">;

def SHR : InstFixed<0xF6, "shr">;
def SHRC : InstFixed<0x76, "shrc">;
def SHL : InstFixed<0xFE, "shl">;
def SHLC : InstFixed<0x7E, "shlc">;

//===----------------------------------------------------------------------===//
// Branch and Flow Control
//===----------------------------------------------------------------------===//

def BR : InstBranch8<0x30, "br">;
def BZ : InstBranch8<0x32, "bz">;
def BNZ : InstBranch8<0x3A, "bnz">;
def BDF : InstBranch8<0x33, "bdf">;
def BNF : InstBranch8<0x3B, "bnf">;

def LBR : InstBranch16<0xC0, "lbr">;
def LBZ : InstBranch16<0xCA, "lbz">;
def LBNZ : InstBranch16<0xC2, "lbnz">;
def LBDF : InstBranch16<0xCB, "lbdf">;
def LBNF : InstBranch16<0xC3, "lbnf">;

def RET : InstFixed<0x70, "ret"> {
  let isReturn = true;
  let isTerminator = true;
  let isBarrier = true;
}

def DIS : InstFixed<0x71, "dis"> {
  let isReturn = true;
  let isTerminator = true;
  let isBarrier = true;
}

//===----------------------------------------------------------------------===//
// Miscellaneous
//===----------------------------------------------------------------------===//

def IDL : InstFixed<0x00, "idl">;
def NOP : InstFixed<0xC4, "nop">;

} // Predicates = [HasRCA1802]
