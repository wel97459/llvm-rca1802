//===-- RCA1802MachineScheduler.h - RCA1802 Instruction Scheduler -------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 machine instruction scheduler.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802_MACHINESCHEDULER_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802_MACHINESCHEDULER_H

#include "llvm/CodeGen/MachineScheduler.h"

namespace llvm {

class RCA1802SchedStrategy : public GenericScheduler {
public:
  RCA1802SchedStrategy(const MachineSchedContext *C);

  bool tryCandidate(SchedCandidate &Cand, SchedCandidate &TryCand,
                    SchedBoundary *Zone) const override;

  int registerClassPressureDiff(const TargetRegisterClass &RC, const SUnit *SU,
                                bool IsTop) const;
};

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802_MACHINESCHEDULER_H
