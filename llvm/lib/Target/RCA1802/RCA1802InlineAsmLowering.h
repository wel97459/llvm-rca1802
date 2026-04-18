//===- RCA1802InlineAsmLowering.h -----------------------------------*- C++ -*-===//
//
// Part of the LLVM-RCA1802 Project, under the Apache License v2.0 with LLVM
// Exceptions. See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file describes how to lower LLVM inline asm to machine code INLINEASM.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INLINEASMLOWERING_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INLINEASMLOWERING_H

#include "RCA1802ISelLowering.h"
#include "llvm/CodeGen/GlobalISel/InlineAsmLowering.h"

namespace llvm {

class RCA1802TargetLowering;

class RCA1802InlineAsmLowering : public InlineAsmLowering {
public:
  RCA1802InlineAsmLowering(RCA1802TargetLowering *TLI);

  bool
  lowerAsmOperandForConstraint(Value *Val, StringRef Constraint,
                               std::vector<MachineOperand> &Ops,
                               MachineIRBuilder &MIRBuilder) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_RCA1802_RCA1802INLINEASMLOWERING_H
