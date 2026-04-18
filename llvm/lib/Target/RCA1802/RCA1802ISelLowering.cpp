//===-- RCA1802ISelLowering.cpp - RCA1802 DAG Lowering Implementation -------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802ISelLowering.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "RCA1802TargetMachine.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

RCA1802TargetLowering::RCA1802TargetLowering(const RCA1802TargetMachine &TM,
                                     const RCA1802Subtarget &STI)
    : TargetLowering(TM, STI) {
  addRegisterClass(MVT::i8, &RCA1802::Anyi8RegClass);
  addRegisterClass(MVT::i16, &RCA1802::GPR16RegClass);
  computeRegisterProperties(STI.getRegisterInfo());

  // R2 is the Stack Pointer according to our convention
  setStackPointerRegisterToSaveRestore(RCA1802::R2);
}

MVT RCA1802TargetLowering::getRegisterType(MVT VT) const {
  if (VT.getSizeInBits() <= 8)
    return MVT::i8;
  return MVT::i16;
}

unsigned RCA1802TargetLowering::getNumRegisters(LLVMContext &Context, EVT VT,
                                            std::optional<MVT> RegisterVT) const {
  if (VT.getSizeInBits() <= 8)
    return 1;
  if (VT.getSizeInBits() <= 16)
    return 1;
  return (VT.getSizeInBits() + 15) / 16;
}

unsigned RCA1802TargetLowering::getNumRegistersForInlineAsm(LLVMContext &Context,
                                                        EVT VT) const {
  return getNumRegisters(Context, VT);
}

RCA1802TargetLowering::ConstraintType RCA1802TargetLowering::getConstraintType(StringRef Constraint) const {
  return TargetLowering::getConstraintType(Constraint);
}

MVT RCA1802TargetLowering::getRegisterTypeForCallingConv(
    LLVMContext &Context, CallingConv::ID CC, EVT VT,
    const ISD::ArgFlagsTy &Flags) const {
  return getRegisterType(VT.getSimpleVT());
}

unsigned
RCA1802TargetLowering::getNumRegistersForCallingConv(LLVMContext &Context,
                                                 CallingConv::ID CC, EVT VT,
                                                 const ISD::ArgFlagsTy &Flags) const {
  return getNumRegisters(Context, VT);
}

std::pair<unsigned, const TargetRegisterClass *>
RCA1802TargetLowering::getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                                StringRef Constraint,
                                                MVT VT) const {
  return TargetLowering::getRegForInlineAsmConstraint(TRI, Constraint, VT);
}

bool RCA1802TargetLowering::isSuitableForJumpTable(const SwitchInst *SI,
                                               uint64_t NumCases,
                                               uint64_t Range,
                                               ProfileSummaryInfo *PSI,
                                               BlockFrequencyInfo *BFI) const {
  return false;
}

bool RCA1802TargetLowering::isLegalAddressingMode(const DataLayout &DL,
                                              const AddrMode &AM, Type *Ty,
                                              unsigned AddrSpace,
                                              Instruction *I) const {
  // RCA1802 has very simple addressing: LDN R(N), STR R(N)
  return AM.Scale == 0 && AM.BaseOffs == 0;
}

bool RCA1802TargetLowering::isTruncateFree(Type *FromTy, Type *ToTy) const {
  return false;
}
bool RCA1802TargetLowering::isTruncateFree(LLT FromTy, LLT ToTy,
                                       LLVMContext &Ctx) const {
  return false;
}

bool RCA1802TargetLowering::isZExtFree(Type *FromTy, Type *ToTy) const {
  return false;
}
bool RCA1802TargetLowering::isZExtFree(LLT FromTy, LLT ToTy,
                                   LLVMContext &Ctx) const {
  return false;
}

MachineBasicBlock *
RCA1802TargetLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                               MachineBasicBlock *MBB) const {
  return MBB;
}
