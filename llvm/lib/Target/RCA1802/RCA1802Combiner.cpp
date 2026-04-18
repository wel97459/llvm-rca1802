//===-- RCA1802Combiner.cpp -----------------------------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802Combiner.h"
#include "RCA1802Subtarget.h"
#include "llvm/CodeGen/GlobalISel/Combiner.h"
#include "llvm/CodeGen/MachineFunctionPass.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-combiner"

namespace {
class RCA1802Combiner : public MachineFunctionPass {
public:
  static char ID;

  RCA1802Combiner() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override { return "RCA1802 Combiner"; }

  bool runOnMachineFunction(MachineFunction &MF) override;
};
} // end anonymous namespace

char RCA1802Combiner::ID = 0;

bool RCA1802Combiner::runOnMachineFunction(MachineFunction &MF) {
  return false;
}

namespace llvm {
FunctionPass *createRCA1802Combiner() { return new RCA1802Combiner(); }
} // namespace llvm
