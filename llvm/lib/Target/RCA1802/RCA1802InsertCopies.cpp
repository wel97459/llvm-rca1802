//===-- RCA1802InsertCopies.cpp - RCA1802 Insert Copies ---------------------------===//
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

#define DEBUG_TYPE "rca1802-insert-copies"

namespace {
class RCA1802InsertCopies : public MachineFunctionPass {
public:
  static char ID;

  RCA1802InsertCopies() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override { return "RCA1802 Insert Copies"; }
};
} // namespace

char RCA1802InsertCopies::ID = 0;

bool RCA1802InsertCopies::runOnMachineFunction(MachineFunction &MF) {
  return false;
}

namespace llvm {
MachineFunctionPass *createRCA1802InsertCopiesPass() {
  return new RCA1802InsertCopies();
}
} // namespace llvm
