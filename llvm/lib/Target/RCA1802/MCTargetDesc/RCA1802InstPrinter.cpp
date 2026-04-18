//===-- RCA1802InstPrinter.cpp - Convert RCA1802 MCInst to assembly syntax --------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802InstPrinter.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "RCA1802GenAsmWriter.inc"

void RCA1802InstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot, const MCSubtargetInfo &STI,
                               raw_ostream &OS) {
  printInstruction(MI, Address, OS);
  printAnnotation(OS, Annot);
}

void RCA1802InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    OS << getRegisterName(Op.getReg(), RCA1802::NoRegAltName);
  } else if (Op.isImm()) {
    OS << Op.getImm();
  } else {
    assert(Op.isExpr() && "unknown operand kind in printOperand");
    MAI.printExpr(OS, *Op.getExpr());
  }
}

void RCA1802InstPrinter::printRegN(const MCInst *MI, unsigned OpNo,
                               raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    StringRef Name = getRegisterName(Op.getReg(), RCA1802::NoRegAltName);
    // RCA1802 registers are r0, r1, ... r15.
    // We want to print just the index (0-15) for instructions like LDN N.
    if (Name.starts_with("r")) {
      StringRef Index = Name.substr(1);
      // If it's r1.0 or r1.1, take only the number
      size_t DotPos = Index.find('.');
      if (DotPos != StringRef::npos)
          Index = Index.substr(0, DotPos);
      OS << Index;
    } else {
      OS << Name;
    }
  } else {
    printOperand(MI, OpNo, OS);
  }
}

} // end namespace llvm
