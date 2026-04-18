//===-- RCA1802LowerSelect.cpp - RCA1802 Lower Select -----------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-lower-select"

namespace {
class RCA1802LowerSelect : public MachineFunctionPass {
public:
  static char ID;

  RCA1802LowerSelect() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override { return "RCA1802 Lower Select"; }
};
} // namespace

char RCA1802LowerSelect::ID = 0;

bool RCA1802LowerSelect::runOnMachineFunction(MachineFunction &MF) {
  return false;
}

namespace llvm {
FunctionPass *createRCA1802LowerSelectPass() { return new RCA1802LowerSelect(); }
} // namespace llvm
