//===-- RCA1802RegisterInfo.td - RCA1802 Register defs -------------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Subregister indices
//===----------------------------------------------------------------------===//

def AliasNames : RegAltNameIndex;

class RCA1802SubRegIndex<int size, int offset = 0>: SubRegIndex<size, offset> {
  let Namespace = "RCA1802";
}

def sublo : RCA1802SubRegIndex<8>;
def subhi : RCA1802SubRegIndex<8, 8>;

//===----------------------------------------------------------------------===//
//  Real registers
//===----------------------------------------------------------------------===//

class RCA1802Reg<bits<16> num, string name, list<string> altNames = []>
  : Register<name>, DwarfRegNum<[num]> {
  field bits<16> Num = num;
  let HWEncoding = num;
  let Namespace = "RCA1802";
  let RegAltNameIndices = !if(!empty(altNames), []<RegAltNameIndex>, [AliasNames]);
  let AltNames = altNames;
}

// 8-bit Accumulator (Data register)
def D : RCA1802Reg<0, "d">;

// 1-bit Data Flag (Carry)
def DF : RCA1802Reg<1, "df">;

// 4-bit Designator registers
def P : RCA1802Reg<2, "p">;
def X : RCA1802Reg<3, "x">;

// 1-bit Q flip-flop
def Q : RCA1802Reg<4, "q">;

// 16-bit Scratchpad Registers R0-R15
foreach I = 0-15 in {
  defvar RegName = "r"#!cast<string>(I);
  defvar AltNames = !cond(
    !eq(I, 2) : ["sp"],
    !eq(I, 3) : ["pc"],
    !eq(I, 4) : ["rcall"],
    !eq(I, 5) : ["rret"],
    !eq(I, 6) : ["retaddr"],
    true : []<string>
  );

  def R#I#_LO : RCA1802Reg<!add(I, 100), RegName#".0">;
  def R#I#_HI : RCA1802Reg<!add(I, 120), RegName#".1">;
  def R#I : RCA1802Reg<I, RegName, AltNames> {
    let SubRegs = [!cast<Register>("R"#!cast<string>(I)#"_LO"),
                   !cast<Register>("R"#!cast<string>(I)#"_HI")];
    let SubRegIndices = [sublo, subhi];
    let CoveredBySubRegs = 1;
  }
}

// Dummy/Fake registers for TableGen compatibility
def Fake : RCA1802Reg<200, "fake">;

// Named aliases for easier reference in C++ and other TD files
class InternalSymbol<string reg> {
  string Reg = reg;
}
def SP : InternalSymbol<"R2">;
def RPC : InternalSymbol<"R3">;

//===----------------------------------------------------------------------===//
//  Register classes
//===----------------------------------------------------------------------===//

class RCA1802RegClass<list<ValueType> regTypes, int alignment, dag regList>
     : RegisterClass<"RCA1802", regTypes, alignment, regList>;

// Core register classes
def GPR16 : RCA1802RegClass<[i16], 8, (add (sequence "R%u", 7, 15), R1, R0, R2, R3, R4, R5, R6)>;

// MOS-compatible register classes (expected by copied C++ code)
def Ac : RCA1802RegClass<[i8], 8, (add D)>;
def Xc : RCA1802RegClass<[i8], 8, (add X)>;
def Pc : RCA1802RegClass<[i8], 8, (add P)>;
def Cc : RCA1802RegClass<[i1], 8, (add DF)>;
def Vc : RCA1802RegClass<[i1], 8, (add Q)>;
def Yc : RCA1802RegClass<[i8], 8, (add X)>;
def XY : RCA1802RegClass<[i8], 8, (add X, P)>;
def GPR : RCA1802RegClass<[i8], 8, (add D, X, P)>;

def Anyi1 : RCA1802RegClass<[i1], 8, (add DF, Q)>;
def Anyi8 : RCA1802RegClass<[i8], 8, (add D, X, P)>;

def Imag8 : RCA1802RegClass<[i8], 8, (add D)>;
def AImag8 : RCA1802RegClass<[i8], 8, (add D)>;
def Imag16 : RCA1802RegClass<[i16], 8, (add R0)>;

let isAllocatable = false in {
  def Fakei32 : RCA1802RegClass<[i32], 8, (add Fake)>;
  def Fakei64 : RCA1802RegClass<[i64], 8, (add Fake)>;
}

// Stack Pointer class (fixed)
def SPClass : RCA1802RegClass<[i16], 8, (add R2)>;

// All 16-bit registers (for assembly/manual use)
def ALL16 : RCA1802RegClass<[i16], 8, (sequence "R%u", 0, 15)>;

def GPR8LO : RCA1802RegClass<[i8], 8, (sequence "R%u_LO", 0, 15)>;
def GPR8HI : RCA1802RegClass<[i8], 8, (sequence "R%u_HI", 0, 15)>;

def Flag : RCA1802RegClass<[i1], 8, (add DF, Q)>;
def Designator : RCA1802RegClass<[i8], 8, (add P, X)>;
