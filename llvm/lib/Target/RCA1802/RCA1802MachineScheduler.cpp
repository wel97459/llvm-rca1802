//===-- RCA1802MachineScheduler.cpp - RCA1802 Instruction Scheduler ---------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the RCA1802 machine instruction scheduler.
//
// The 6502 has no cache in any of its common configurations. This means that
// every instruction is loaded from memory before being executed. Essentially,
// this means that its pipeline stalls after every instruction. Accordingly, it
// doesn't matter one bit for throughput what order instructions execute in, so
// rca1802t instruction scheduling concerns are totally irrelevant.
//
// There is one important exception: register pressure. The order of
// instructions can make enormous differences in the number of registers
// required to execute a basic block. For example, it's often possible to order
// arthmetic in such a way that temporaries are threaded through the
// instructions entirely in the A register. Order the instructions differently,
// and the live ranges may begin to overlap, requiring a huge number of
// additional temporary locations.
//
// Thus, the RCA1802 scheduling strategy more or less copies just the register
// pressure parts of the standard Machine Scheduler.
//
//===----------------------------------------------------------------------===//

#include "RCA1802MachineScheduler.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802RegisterInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineScheduler.h"

using namespace llvm;

RCA1802SchedStrategy::RCA1802SchedStrategy(const MachineSchedContext *C)
    : GenericScheduler(C) {}

bool RCA1802SchedStrategy::tryCandidate(SchedCandidate &Cand,
                                    SchedCandidate &TryCand,
                                    SchedBoundary *Zone) const {

  // Initialize the candidate if needed.
  if (!Cand.isValid()) {
    TryCand.Reason = NodeOrder;
    return true;
  }

  if (tryLess(
          registerClassPressureDiff(RCA1802::AcRegClass, TryCand.SU, TryCand.AtTop),
          registerClassPressureDiff(RCA1802::AcRegClass, Cand.SU, Cand.AtTop),
          TryCand, Cand, PhysReg))
    return TryCand.Reason != NoCand;

  if (tryLess(
          registerClassPressureDiff(RCA1802::XYRegClass, TryCand.SU, TryCand.AtTop),
          registerClassPressureDiff(RCA1802::XYRegClass, Cand.SU, Cand.AtTop),
          TryCand, Cand, PhysReg))
    return TryCand.Reason != NoCand;

  if (tryLess(
          registerClassPressureDiff(RCA1802::Imag8RegClass, TryCand.SU,
                                    TryCand.AtTop),
          registerClassPressureDiff(RCA1802::Imag8RegClass, Cand.SU, Cand.AtTop),
          TryCand, Cand, PhysReg))
    return TryCand.Reason != NoCand;

  // Avoid exceeding the target's limit.
  if (DAG->isTrackingPressure() &&
      tryPressure(TryCand.RPDelta.Excess, Cand.RPDelta.Excess, TryCand, Cand,
                  RegExcess, TRI, DAG->MF))
    return TryCand.Reason != NoCand;

  // Avoid increasing the max critical pressure in the scheduled region.
  if (DAG->isTrackingPressure() &&
      tryPressure(TryCand.RPDelta.CriticalMax, Cand.RPDelta.CriticalMax,
                  TryCand, Cand, RegCritical, TRI, DAG->MF))
    return TryCand.Reason != NoCand;

  // Avoid increasing the max pressure of the entire region.
  if (DAG->isTrackingPressure() &&
      tryPressure(TryCand.RPDelta.CurrentMax, Cand.RPDelta.CurrentMax, TryCand,
                  Cand, RegMax, TRI, DAG->MF))
    return TryCand.Reason != NoCand;

  // We only compare a subset of features when comparing nodes between
  // Top and Bottom boundary. Some properties are simply incomparable, in many
  // other instances we should only override the other boundary if something
  // is a clear good pick on one boundary. Skip heuristics that are more
  // "tie-breaking" in nature.
  bool SameBoundary = Zone != nullptr;
  if (SameBoundary) {
    // Fall through to original instruction order.
    if ((Zone->isTop() && TryCand.SU->NodeNum < Cand.SU->NodeNum) ||
        (!Zone->isTop() && TryCand.SU->NodeNum > Cand.SU->NodeNum)) {
      TryCand.Reason = NodeOrder;
      return true;
    }
  }

  return false;
}

// Returns the change in pressure in a SU for a physical register.
int RCA1802SchedStrategy::registerClassPressureDiff(const TargetRegisterClass &RC,
                                                const SUnit *SU,
                                                bool IsTop) const {
  const MachineInstr *MI = SU->getInstr();

  int PressureDiff = 0;
  for (const MachineOperand &MO : MI->operands()) {
    if (!MO.isReg() || !MO.getReg().isPhysical() || !RC.contains(MO.getReg()))
      continue;
    if (MO.isDef()) {
      PressureDiff += IsTop ? 1 : -1;
    } else {
      PressureDiff += IsTop ? -1 : 1;
    }
  }
  return PressureDiff;
}
