//===-- RCA1802.td - Describe the RCA1802 Target Machine ----------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//
// This is the top level entry point for the RCA1802 target.
//===---------------------------------------------------------------------===//

include "llvm/Target/Target.td"
include "llvm/Target/GlobalISel/Combine.td"
include "llvm/TableGen/SearchableTable.td"

//===---------------------------------------------------------------------===//
// RCA1802 Target Definitions
//===---------------------------------------------------------------------===//

include "RCA1802Features.td"
include "RCA1802Devices.td"
include "RCA1802RegisterInfo.td"
include "RCA1802RegisterBanks.td"
include "RCA1802InstrFormats.td"
include "RCA1802InstrGISel.td"
include "RCA1802InstrInfo.td"
include "RCA1802InstrPseudos.td"
include "RCA1802InstrInfoTables.td"
include "RCA1802CallingConv.td"
include "RCA1802Combine.td"

def RCA1802InstrInfo : InstrInfo {
  let guessInstructionProperties = 0;
}

def RCA1802AsmWriter : AsmWriter {
  string AsmWriterClassName = "InstPrinter";
  bit isMCAsmWriter = 1;
}

def RCA1802AsmParser : AsmParser {
  let ReportMultipleNearMisses = 1;
  string RegisterPrefix = "";
  string TokenizingCharacters = "#(),[]*!";
}

def RCA1802AsmParserVariantDefault : AsmParserVariant {
  int Variant = 0;
  let Name = "generic";
}

//===---------------------------------------------------------------------===//
// Target Declaration
//===---------------------------------------------------------------------===//

def RCA1802 : Target {
  let InstructionSet         = RCA1802InstrInfo;
  let AssemblyWriters        = [RCA1802AsmWriter];
  let AssemblyParsers        = [RCA1802AsmParser];
  let AssemblyParserVariants = [RCA1802AsmParserVariantDefault];
  let AllowRegisterRenaming = 1;
}

// Map soft stack/frame pointers to specific RCA1802 registers if needed by logic
defm : RemapAllTargetPseudoPointerOperands<GPR16>;
