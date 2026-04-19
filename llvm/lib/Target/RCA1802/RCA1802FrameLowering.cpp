//===-- RCA1802FrameLowering.cpp - RCA1802 Frame Lowering
//-------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the RCA1802 implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "RCA1802FrameLowering.h"
#include "RCA1802InstrBuilder.h"
#include "RCA1802InstrInfo.h"
#include "RCA1802MachineFunctionInfo.h"
#include "RCA1802Subtarget.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

using namespace llvm;

RCA1802FrameLowering::RCA1802FrameLowering(const RCA1802Subtarget &STI)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(1), 0,
                          Align(1)) {}

bool RCA1802FrameLowering::hasFPImpl(const MachineFunction &MF) const {
  return false;
}

void RCA1802FrameLowering::emitPrologue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {}

void RCA1802FrameLowering::emitEpilogue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {
  const RCA1802InstrInfo &TII =
      *MF.getSubtarget<RCA1802Subtarget>().getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  BuildMI(MBB, MBBI, DL, TII.get(RCA1802::RET_PSEUDO))
      .setMIFlag(MachineInstr::FrameDestroy);
}

bool RCA1802FrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  return false;
}

bool RCA1802FrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    MutableArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  return false;
}

void RCA1802FrameLowering::determineCalleeSaves(MachineFunction &MF,
                                                BitVector &SavedRegs,
                                                RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
}

void RCA1802FrameLowering::processFunctionBeforeFrameFinalized(
    MachineFunction &MF, RegScavenger *RS) const {}

void RCA1802FrameLowering::offsetSP(MachineIRBuilder &Builder,
                                    int64_t Offset) const {}
