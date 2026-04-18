//===-- RCA1802InstrInfoTables.td - RCA1802 Instruction tables -----*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class RCA1802RelaxationEntry<Instruction from, Instruction to> {
  Instruction From = from;
  Instruction To = to;
}

class ZeroPageInstructionRelaxationEntry<Instruction from, Instruction to> : RCA1802RelaxationEntry<from, to>;
class ZeroBankInstructionRelaxationEntry<Instruction from, Instruction to> : RCA1802RelaxationEntry<from, to>;
class BranchInstructionRelaxationEntry<Instruction from, Instruction to> : RCA1802RelaxationEntry<from, to>;

class ZPIRE<Instruction from, Instruction to> : ZeroPageInstructionRelaxationEntry<from, to>;
class ZBIRE<Instruction from, Instruction to> : ZeroBankInstructionRelaxationEntry<from, to>;
class BIRE<Instruction from, Instruction to> : BranchInstructionRelaxationEntry<from, to>;

// Branch relaxations (8-bit to 16-bit)
def : BIRE<BR, LBR>;
def : BIRE<BZ, LBZ>;
def : BIRE<BNZ, LBNZ>;
def : BIRE<BDF, LBDF>;
def : BIRE<BNF, LBNF>;

// Dummy entries for other tables to satisfy TableGen
def : ZPIRE<LDI, LDI>;
def : ZBIRE<LDI, LDI>;

class RCA1802RelaxationTable : GenericTable {
  let FilterClass = NAME # "Entry";
  let CppTypeName = "InstructionRelaxationEntry";
  let Fields = ["From", "To"];
  let PrimaryKey = ["From"];
  let PrimaryKeyName = "get" # NAME # "Entry";
}

def ZeroPageInstructionRelaxation : RCA1802RelaxationTable;
def ZeroBankInstructionRelaxation : RCA1802RelaxationTable;
def BranchInstructionRelaxation : RCA1802RelaxationTable;
