//===-- RCA1802Subtarget.h - RCA1802 Subtarget Information -------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802SUBTARGET_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802SUBTARGET_H

#include "RCA1802CallLowering.h"
#include "RCA1802FrameLowering.h"
#include "RCA1802ISelLowering.h"
#include "RCA1802InstrInfo.h"
#include "RCA1802LegalizerInfo.h"
#include "RCA1802RegisterBankInfo.h"
#include "RCA1802RegisterInfo.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/TargetParser/Triple.h"

#define GET_SUBTARGETINFO_HEADER
#include "RCA1802GenSubtargetInfo.inc"

namespace llvm {
class RCA1802TargetMachine;

class RCA1802Subtarget : public RCA1802GenSubtargetInfo {
public:
  RCA1802Subtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
               const RCA1802TargetMachine &TM);

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);

  const RCA1802InstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const RCA1802FrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const RCA1802TargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const RCA1802RegisterInfo *getRegisterInfo() const override {
    return &RegInfo;
  }
  const CallLowering *getCallLowering() const override {
    return &CallLoweringInfo;
  }
  const LegalizerInfo *getLegalizerInfo() const override {
    return &Legalizer;
  }
  const RCA1802RegisterBankInfo *getRegBankInfo() const override {
    return &RegBankInfo;
  }

  bool hasRCA1802() const;
  bool hasRCA1804() const;

  // Feature accessors
  bool hasW65816() const { return HasW65816; }
  bool has65CE02() const { return Has65CE02; }
  bool has65EL02() const { return Has65EL02; }
  bool hasHUC6280() const { return HasHUC6280; }
  bool hasSPC700() const { return HasSPC700; }
  bool hasJMPIdxIndir() const { return HasJMPIdxIndir; }
  bool has45GS02() const { return Has45GS02; }
  bool hasGPRIncDec() const { return HasGPRIncDec; }
  bool staticStack() const { return StaticStack; }

private:
  // Features (referenced by GenSubtargetInfo.inc)
  bool HasRCA1802 = false;
  bool HasRCA1804 = false;
  bool HasW65816 = false;
  bool Has65CE02 = false;
  bool Has65EL02 = false;
  bool HasHUC6280 = false;
  bool HasSPC700 = false;
  bool HasJMPIdxIndir = false;
  bool Has45GS02 = false;
  bool HasGPRIncDec = false;
  bool StaticStack = false;

  RCA1802RegisterInfo RegInfo;
  RCA1802FrameLowering FrameLowering;
  RCA1802InstrInfo InstrInfo;
  RCA1802TargetLowering TLInfo;
  RCA1802CallLowering CallLoweringInfo;
  RCA1802LegalizerInfo Legalizer;
  RCA1802RegisterBankInfo RegBankInfo;
};
} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802SUBTARGET_H
