//===-- RCA1802Subtarget.cpp - RCA1802 Subtarget Information ----------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the RCA1802 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "RCA1802Subtarget.h"
#include "RCA1802.h"
#include "RCA1802RegisterBankInfo.h"
#include "RCA1802TargetMachine.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "RCA1802GenSubtargetInfo.inc"

RCA1802Subtarget::RCA1802Subtarget(const Triple &TT, const std::string &CPU,
                               const std::string &FS,
                               const RCA1802TargetMachine &TM)
    : RCA1802GenSubtargetInfo(TT, CPU, CPU, FS),
      RegInfo(), FrameLowering(*this), InstrInfo(*this), TLInfo(TM, *this),
      CallLoweringInfo(&TLInfo), Legalizer(*this), RegBankInfo() {
  ParseSubtargetFeatures(CPU, CPU, FS);
}

bool RCA1802Subtarget::hasRCA1802() const { return HasRCA1802; }
bool RCA1802Subtarget::hasRCA1804() const { return HasRCA1804; }
