//===-- RCA1802MCInstrAnalysis.h - RCA1802 instruction analysis ---------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the RCA1802MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RCA1802_MC_INSTR_ANALYSIS_H
#define LLVM_RCA1802_MC_INSTR_ANALYSIS_H

#include "llvm/MC/MCInstrAnalysis.h"

namespace llvm {

class Triple;

class RCA1802MCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit RCA1802MCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override;

  std::optional<uint64_t>
  evaluateMemoryOperandAddress(const MCInst &Inst, const MCSubtargetInfo *STI,
                               uint64_t Addr, uint64_t Size) const override;
};

} // end namespace llvm

#endif // LLVM_RCA1802_MC_INSTR_ANALYSIS_H
