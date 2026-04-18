//===-- RCA1802InstructionSelector.h - RCA1802 Instruction Selector -----*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 instruction selector.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INSTRUCTIONSELECTOR_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INSTRUCTIONSELECTOR_H

#include "RCA1802TargetMachine.h"
#include "RCA1802RegisterBankInfo.h"
#include "RCA1802Subtarget.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"

namespace llvm {

InstructionSelector *
createRCA1802InstructionSelector(const RCA1802TargetMachine &TM,
                                 RCA1802Subtarget &STI,
                                 RCA1802RegisterBankInfo &RBI);

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802INSTRUCTIONSELECTOR_H
