//===-- RCA1802MCInstrAnalysis.cpp - RCA1802 instruction analysis -----------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802MCInstrAnalysis.h"
#include "RCA1802MCTargetDesc.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

bool RCA1802MCInstrAnalysis::evaluateBranch(const MCInst &Inst,
                                        uint64_t Addr,
                                        uint64_t Size,
                                        uint64_t &Target) const {
  if ((!isBranch(Inst) && !isCall(Inst)) || isIndirectBranch(Inst))
    return false;
  unsigned NumOps = Inst.getNumOperands();
  if (NumOps == 0)
    return false;

  const MCOperand &Op = Inst.getOperand(NumOps - 1);
  if (!Op.isImm())
    return false;

  const auto &Desc = Info->get(Inst.getOpcode());
  switch (Desc.operands()[NumOps - 1].OperandType) {
    case RCA1802Op::OPERAND_ADDR16:
    case RCA1802Op::OPERAND_ADDR24: {
      Target = Op.getImm();
      return true;
    }
    case MCOI::OPERAND_PCREL: {
      Target = Addr + Size + Op.getImm();
      return true;
    }
  }
  return false;
}

} //  namespace llvm
