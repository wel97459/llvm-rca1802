//===-- RCA1802ZeroPageAlloc.cpp - RCA1802 Zero Page Allocation -------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802.h"
#include "RCA1802Subtarget.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/Pass.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-zero-page-alloc"

namespace {
class RCA1802ZeroPageAlloc : public ModulePass {
public:
  static char ID;

  RCA1802ZeroPageAlloc() : ModulePass(ID) {}

  bool runOnModule(Module &M) override;

  StringRef getPassName() const override { return "RCA1802 Zero Page Allocation"; }
};
} // namespace

char RCA1802ZeroPageAlloc::ID = 0;

bool RCA1802ZeroPageAlloc::runOnModule(Module &M) {
  return false;
}

namespace llvm {
ModulePass *createRCA1802ZeroPageAllocPass() {
  return new RCA1802ZeroPageAlloc();
}
} // namespace llvm
