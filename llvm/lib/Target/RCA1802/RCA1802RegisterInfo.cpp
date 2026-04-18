//===-- RCA1802RegisterInfo.cpp - RCA1802 Register Information --------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the RCA1802 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "RCA1802RegisterInfo.h"
#include "RCA1802.h"
#include "RCA1802Subtarget.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "RCA1802GenRegisterInfo.inc"

RCA1802RegisterInfo::RCA1802RegisterInfo()
    : RCA1802GenRegisterInfo(RCA1802::R3) { // RPC is Program Counter
}

const MCPhysReg *
RCA1802RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return RCA1802_CSR_SaveList;
}

const uint32_t *
RCA1802RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                     CallingConv::ID) const {
  return RCA1802_CSR_RegMask;
}

BitVector RCA1802RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(RCA1802::R0); // Often reserved for DMA or other system uses
  Reserved.set(RCA1802::R1); // Often reserved
  Reserved.set(RCA1802::R2); // Stack Pointer
  Reserved.set(RCA1802::R3); // Program Counter
  return Reserved;
}

const TargetRegisterClass *
RCA1802RegisterInfo::getLargestLegalSuperClass(const TargetRegisterClass *RC,
                                           const MachineFunction &) const {
  if (RC->hasSuperClassEq(&RCA1802::GPR16RegClass))
    return &RCA1802::GPR16RegClass;
  if (RC->hasSuperClassEq(&RCA1802::Anyi8RegClass))
    return &RCA1802::Anyi8RegClass;
  return RC;
}

const TargetRegisterClass *
RCA1802RegisterInfo::getCrossCopyRegClass(const TargetRegisterClass *RC) const {
  return RC;
}

unsigned RCA1802RegisterInfo::getCSRCost(const MachineFunction &MF) const {
  return 1;
}

bool RCA1802RegisterInfo::saveScavengerRegister(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
    MachineBasicBlock::iterator &UseMI, const TargetRegisterClass *RC,
    Register Reg) const {
  return false;
}

bool RCA1802RegisterInfo::canSaveScavengerRegister(
    Register Reg, MachineBasicBlock::iterator I,
    MachineBasicBlock::iterator UseMI) const {
  return false;
}

bool RCA1802RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  return false;
}

Register RCA1802RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return RCA1802::R2; // Use SP as FP for now
}

bool RCA1802RegisterInfo::shouldCoalesce(MachineInstr *MI,
                                     const TargetRegisterClass *SrcRC,
                                     unsigned SubReg,
                                     const TargetRegisterClass *DstRC,
                                     unsigned DstSubReg,
                                     const TargetRegisterClass *NewRC,
                                     LiveIntervals &LIS) const {
  return true;
}

bool RCA1802RegisterInfo::getRegAllocationHints(
    Register VirtReg, ArrayRef<MCPhysReg> Order,
    SmallVectorImpl<MCPhysReg> &Hints, const MachineFunction &MF,
    const VirtRegMap *VRM, const LiveRegMatrix *Matrix) const {
  return false;
}
