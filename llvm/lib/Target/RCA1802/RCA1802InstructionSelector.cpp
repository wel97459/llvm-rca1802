//===-- RCA1802InstructionSelector.cpp ------------------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802InstructionSelector.h"
#include "RCA1802.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "RCA1802TargetMachine.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/GlobalISel/GIMatchTableExecutorImpl.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-isel"

namespace {

class RCA1802InstructionSelector : public InstructionSelector {
public:
  RCA1802InstructionSelector(const RCA1802TargetMachine &TM,
                         const RCA1802Subtarget &STI,
                         const RCA1802RegisterBankInfo &RBI);

  bool select(MachineInstr &I) override;
  void setupGeneratedPerFunctionState(MachineFunction &MF) override {}

private:
  const RCA1802InstrInfo &TII;
  const RCA1802RegisterInfo &TRI;
  const RCA1802RegisterBankInfo &RBI;
};

} // end anonymous namespace

RCA1802InstructionSelector::RCA1802InstructionSelector(
    const RCA1802TargetMachine &TM, const RCA1802Subtarget &STI,
    const RCA1802RegisterBankInfo &RBI)
    : InstructionSelector(), TII(*STI.getInstrInfo()), TRI(*STI.getRegisterInfo()), RBI(RBI) {
}

bool RCA1802InstructionSelector::select(MachineInstr &I) {
  return false;
}

namespace llvm {
InstructionSelector *
createRCA1802InstructionSelector(const RCA1802TargetMachine &TM,
                             const RCA1802Subtarget &STI,
                             const RCA1802RegisterBankInfo &RBI) {
  return new RCA1802InstructionSelector(TM, STI, RBI);
}
} // namespace llvm
