//===-- RCA1802ShiftRotateChain.cpp - RCA1802 Shift/Rotate Chaining ---------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file This file defines the RCA1802 shift/rotate chaining pass.
/// These operations take linear time in the amount, so basing one such
/// operation on another of the same type can reduce the overall amount of work
/// needed. This only works when the amount is constant; logic otherwise would
/// be complex and rarely applicable.
//
//===----------------------------------------------------------------------===//

#include "RCA1802ShiftRotateChain.h"

#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802.h"
#include "llvm/ADT/IndexedMap.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define DEBUG_TYPE "rca1802-shift-rotate-chain"

using namespace llvm;

namespace {

class RCA1802ShiftRotateChain : public MachineFunctionPass {
public:
  static char ID;

  RCA1802ShiftRotateChain() : MachineFunctionPass(ID) {
    llvm::initializeRCA1802ShiftRotateChainPass(*PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    MachineFunctionPass::getAnalysisUsage(AU);
    AU.addRequired<MachineDominatorTreeWrapperPass>();
    AU.addPreserved<MachineDominatorTreeWrapperPass>();
  }

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::IsSSA);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  // Move PrevMI and the values it depends on up the dominance tree until it
  // dominates MI, all the way up to base.
  void ensureDominates(const MachineInstr &Base, MachineInstr &PrevMI,
                       MachineInstr &MI) const;
};

bool RCA1802ShiftRotateChain::runOnMachineFunction(MachineFunction &MF) {
  struct ChainEntry {
    Register R;
    unsigned Opcode;
    unsigned Amount;
  };
  typedef SmallVector<ChainEntry> Chain;
  IndexedMap<Chain, VirtReg2IndexFunctor> Chains;

  LLVM_DEBUG(dbgs() << "\n\nChaining shifts and rotates in: " << MF.getName()
                    << "\n\n");

  const MachineRegisterInfo &MRI = MF.getRegInfo();

  Chains.resize(MRI.getNumVirtRegs());
  for (unsigned I = 0, E = MRI.getNumVirtRegs(); I != E; ++I) {
    Register R = Register::index2VirtReg(I);
    MachineInstr *MI = MRI.getUniqueVRegDef(R);
    if (!MI)
      continue;

    unsigned Opcode = MI->getOpcode();
    switch (Opcode) {
    case RCA1802::G_SHL:
    case RCA1802::G_ASHR:
    case RCA1802::G_LSHR:
    case RCA1802::G_ROTL:
    case RCA1802::G_ROTR:
      break;
    default:
      continue;
    }

    Register LHS = MI->getOperand(1).getReg();
    Register RHS = MI->getOperand(2).getReg();
    auto RHSConst = getIConstantVRegValWithLookThrough(RHS, MRI);
    if (!RHSConst)
      continue;
    unsigned RHSValue = RHSConst->Value.getZExtValue();
    Chains[LHS].push_back({R, Opcode, RHSValue});
  }

  for (unsigned I = 0, E = MRI.getNumVirtRegs(); I != E; ++I) {
    Register R = Register::index2VirtReg(I);

    llvm::sort(Chains[R], [](const ChainEntry &L, const ChainEntry &R) {
      if (L.Opcode < R.Opcode)
        return true;
      if (L.Opcode > R.Opcode)
        return false;
      return L.Amount < R.Amount;
    });

    LLVM_DEBUG({
      if (!Chains[R].empty()) {
        const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
        dbgs() << "Creating chain for " << printReg(R) << ":\n";
        for (const ChainEntry &C : Chains[R]) {
          dbgs() << printReg(C.R) << " := " << TII.getName(C.Opcode) << ' '
                 << C.Amount << '\n';
        }
        dbgs() << '\n';
      }
    });
  }

  bool Changed = false;
  for (unsigned I = 0, E = MRI.getNumVirtRegs(); I != E; ++I) {
    Register R = Register::index2VirtReg(I);
    if (Chains[R].empty())
      continue;

    for (unsigned I = 0, E = Chains[R].size(); I != E; ++I) {
      ChainEntry &C = Chains[R][I];
      if (!I || C.Opcode != Chains[R][I - 1].Opcode)
        continue;
      ChainEntry &Prev = Chains[R][I - 1];

      MachineInstr &MI = *MRI.getUniqueVRegDef(C.R);
      MachineInstr &PrevMI = *MRI.getUniqueVRegDef(Prev.R);

      ensureDominates(*MRI.getUniqueVRegDef(R), PrevMI, MI);

      Changed = true;
      MI.getOperand(1).setReg(PrevMI.getOperand(0).getReg());
      MachineIRBuilder B(MI);
      MI.getOperand(2).setReg(
          B.buildConstant(MRI.getType(C.R), C.Amount - Prev.Amount).getReg(0));
    }
  }
  return Changed;
}

void RCA1802ShiftRotateChain::ensureDominates(const MachineInstr &Base,
                                          MachineInstr &PrevMI,
                                          MachineInstr &MI) const {
  auto &MDT = getAnalysis<MachineDominatorTreeWrapperPass>().getDomTree();
  if (MDT.dominates(&PrevMI, &MI))
    return;

  const auto &MRI = PrevMI.getParent()->getParent()->getRegInfo();

  MachineBasicBlock *MBB;
  MachineBasicBlock::iterator InsertPt;
  if (MDT.dominates(&MI, &PrevMI)) {
    MBB = MI.getParent();
    InsertPt = MI;
  } else {
    MBB = MDT.findNearestCommonDominator(PrevMI.getParent(), MI.getParent());
    InsertPt = MBB->getFirstTerminator();
  }

  MBB->insert(InsertPt, PrevMI.removeFromParent());
  PrevMI.setDebugLoc(MBB->findDebugLoc(InsertPt));
  MachineInstr &AmountMI = *MRI.getUniqueVRegDef(PrevMI.getOperand(2).getReg());
  if (!MDT.dominates(&AmountMI, &PrevMI))
    MBB->insert(PrevMI, AmountMI.removeFromParent());

  MachineInstr &ImmBase = *MRI.getUniqueVRegDef(PrevMI.getOperand(1).getReg());
  if (&ImmBase != &Base)
    ensureDominates(Base, ImmBase, PrevMI);
}

} // namespace

char RCA1802ShiftRotateChain::ID = 0;

INITIALIZE_PASS(RCA1802ShiftRotateChain, DEBUG_TYPE, "RCA1802 Shift/Rotate Chaining",
                false, false)

MachineFunctionPass *llvm::createRCA1802ShiftRotateChainPass() {
  return new RCA1802ShiftRotateChain();
}
