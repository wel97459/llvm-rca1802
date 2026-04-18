//===-- RCA1802PostRAScavenging.cpp - RCA1802 Post RA Scavenging ------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the RCA1802 post-register-allocation register scavenging pass.
//
// This pass runs immediately after post-RA pseudo expansion. These pseudos
// (including COPY) often require temporary registers on RCA1802; moreso than on
// other platforms. Accordingly, they emit virtual registers instead, and this
// pass performs register scavenging to assign them to physical registers,
// freeing them up via save and restore if neccesary. A very similar process is
// performed in prologue/epilogue insertion.
//
//===----------------------------------------------------------------------===//

#include "RCA1802PostRAScavenging.h"

#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/RegisterScavenging.h"

#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802.h"

#define DEBUG_TYPE "rca1802-scavenging"

using namespace llvm;

namespace {

class RCA1802PostRAScavenging : public MachineFunctionPass {
public:
  static char ID;

  RCA1802PostRAScavenging() : MachineFunctionPass(ID) {
    llvm::initializeRCA1802PostRAScavengingPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
};

bool RCA1802PostRAScavenging::runOnMachineFunction(MachineFunction &MF) {
  if (MF.getProperties().hasProperty(
          MachineFunctionProperties::Property::NoVRegs))
    return false;

  RegScavenger RS;
  scavengeFrameVirtualRegs(MF, RS);

  return true;
}

} // namespace

char RCA1802PostRAScavenging::ID = 0;

INITIALIZE_PASS(RCA1802PostRAScavenging, DEBUG_TYPE,
                "Scavenge virtual registers emitted by post-RA pseudos", false,
                false)

MachineFunctionPass *llvm::createRCA1802PostRAScavengingPass() {
  return new RCA1802PostRAScavenging();
}
