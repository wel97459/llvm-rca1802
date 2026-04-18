//===-- RCA1802CopyOpt.cpp - RCA1802 Copy Optimization ----------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802CopyOpt.h"
#include "RCA1802.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-copy-opt"

namespace {
class RCA1802CopyOpt : public MachineFunctionPass {
public:
  static char ID;

  RCA1802CopyOpt() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override { return "RCA1802 Copy Optimization"; }
};
} // namespace

char RCA1802CopyOpt::ID = 0;

bool RCA1802CopyOpt::runOnMachineFunction(MachineFunction &MF) {
  bool Changed = false;

  for (auto &MBB : MF) {
    for (auto &MI : MBB) {
      if (!MI.isCopy())
        continue;

      Register Dst = MI.getOperand(0).getReg();
      Register Src = MI.getOperand(1).getReg();

      if (!Dst.isPhysical() || !Src.isPhysical())
        continue;

      if (Dst == Src) {
        MI.eraseFromParent();
        Changed = true;
        break;
      }
    }
  }

  return Changed;
}

namespace llvm {
MachineFunctionPass *createRCA1802CopyOptPass() { return new RCA1802CopyOpt(); }
} // namespace llvm
