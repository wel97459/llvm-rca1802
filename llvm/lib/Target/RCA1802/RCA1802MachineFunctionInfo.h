//===-- RCA1802MachineFuctionInfo.h - RCA1802 machine function info -----*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares RCA1802-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802MACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802MACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class RCA1802Subtarget;

struct RCA1802FunctionInfo : public MachineFunctionInfo {
  RCA1802FunctionInfo(const Function &F, const RCA1802Subtarget *STI) {}

  MachineFunctionInfo *
  clone(BumpPtrAllocator &Allocator, MachineFunction &DestMF,
        const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
      const override {
    return DestMF.cloneInfo<RCA1802FunctionInfo>(*this);
  }

  int VarArgsStackIndex = -1;
  const GlobalValue *StaticStackValue = nullptr;
  const GlobalValue *ZeroPageStackValue = nullptr;
  DenseMap<Register, size_t> CSRZPOffsets;
};

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802MACHINEFUNCTIONINFO_H
