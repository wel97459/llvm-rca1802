//===-- RCA1802LateOptimization.cpp - RCA1802 Late Optimization -------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-late-opt"

namespace {
class RCA1802LateOptimization : public MachineFunctionPass {
public:
  static char ID;

  RCA1802LateOptimization() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override { return "RCA1802 Late Optimization"; }
};
} // namespace

char RCA1802LateOptimization::ID = 0;

bool RCA1802LateOptimization::runOnMachineFunction(MachineFunction &MF) {
  return false;
}

namespace llvm {
FunctionPass *createRCA1802LateOptimizationPass() {
  return new RCA1802LateOptimization();
}
} // namespace llvm
