//===-- RCA1802InstrInfo.cpp - RCA1802 Instruction Information --------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the RCA1802 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "RCA1802InstrInfo.h"
#include "RCA1802.h"
#include "RCA1802Subtarget.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "RCA1802GenInstrInfo.inc"

RCA1802InstrInfo::RCA1802InstrInfo(const RCA1802Subtarget &STI)
    : RCA1802GenInstrInfo(STI, *STI.getRegisterInfo(), RCA1802::ADJCALLSTACKDOWN, RCA1802::ADJCALLSTACKUP, 0, 0),
      STI(STI) {}

void RCA1802InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator MI, const DebugLoc &DL,
                              Register DestReg, Register SrcReg,
                              bool KillSrc, bool RenamableDest,
                              bool RenamableSrc) const {
  BuildMI(MBB, MI, DL, get(TargetOpcode::COPY), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

void RCA1802InstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register SrcReg,
    bool isKill, int FrameIndex, const TargetRegisterClass *RC, Register VReg,
    MachineInstr::MIFlag Flags) const {
}

void RCA1802InstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register DestReg,
    int FrameIndex, const TargetRegisterClass *RC, Register VReg,
    unsigned SubReg, MachineInstr::MIFlag Flags) const {
}

bool RCA1802InstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const {
  return true;
}

bool RCA1802InstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                 MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool AllowModify) const {
  return true;
}

unsigned RCA1802InstrInfo::insertBranch(MachineBasicBlock &MBB,
                                    MachineBasicBlock *TBB,
                                    MachineBasicBlock *FBB,
                                    ArrayRef<MachineOperand> Cond,
                                    const DebugLoc &DL, int *BytesAdded) const {
  return 0;
}

unsigned RCA1802InstrInfo::removeBranch(MachineBasicBlock &MBB,
                                    int *BytesRemoved) const {
  return 0;
}
