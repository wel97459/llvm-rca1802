//===-- RCA1802ISelLowering.h - RCA1802 DAG Lowering Interface ----------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that RCA1802 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802ISELLOWERING_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802ISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

#include "llvm/Target/TargetMachine.h"

namespace llvm {

class RCA1802Subtarget;
class RCA1802TargetMachine;

class RCA1802TargetLowering : public TargetLowering {
public:
  RCA1802TargetLowering(const RCA1802TargetMachine &TM, const RCA1802Subtarget &STI);

  bool isSelectSupported(SelectSupportKind /*kind*/) const override {
    return false;
  }

  // While integer division isn't "cheap", long division is not all that much
  // slower than long multiplication, and the division->multiplication
  // optimization this disables performs multiplciation at double the width,
  // which is extraordinarily more expensive.
  bool isIntDivCheap(EVT VT, AttributeList Attr) const override { return true; }

  bool areJTsAllowed(const Function *Fn) const override {
    return !Fn->getFnAttribute("no-jump-tables").getValueAsBool();
  }

  bool isSuitableForJumpTable(const SwitchInst *SI, uint64_t NumCases,
                              uint64_t Range, ProfileSummaryInfo *PSI,
                              BlockFrequencyInfo *BFI) const override;

  MVT getRegisterType(MVT VT) const override;

  unsigned
  getNumRegisters(LLVMContext &Context, EVT VT,
                  std::optional<MVT> RegisterVT = std::nullopt) const override;

  unsigned getNumRegistersForInlineAsm(LLVMContext &Context,
                                       EVT VT) const override;

  ConstraintType getConstraintType(StringRef Constraint) const override;

  MVT getRegisterTypeForCallingConv(
      LLVMContext &Context, CallingConv::ID CC, EVT VT,
      const ISD::ArgFlagsTy &Flags) const override;

  unsigned
  getNumRegistersForCallingConv(LLVMContext &Context, CallingConv::ID CC,
                                EVT VT,
                                const ISD::ArgFlagsTy &Flags) const override;

  std::pair<unsigned, const TargetRegisterClass *>
  getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                               StringRef Constraint, MVT VT) const override;

  bool isLegalAddressingMode(const DataLayout &DL, const AddrMode &AM, Type *Ty,
                             unsigned AddrSpace,
                             Instruction *I = nullptr) const override;

  bool isTruncateFree(Type *FromTy, Type *ToTy) const override;
  bool isTruncateFree(LLT FromTy, LLT ToTy, LLVMContext &Ctx) const override;

  bool isZExtFree(Type *FromTy, Type *ToTy) const override;
  bool isZExtFree(LLT FromTy, LLT ToTy, LLVMContext &Ctx) const override;

  bool preferNarrowTypes() const override { return true; }

  CallingConv::ID getCallingConv() const { return CallingConv::C; }

  EVT getOptimalMemOpType(LLVMContext &Context, const MemOp &Op,
                          const AttributeList &FuncAttributes) const override {
    return MVT::i8;
  }

  LLT getOptimalMemOpLLT(const MemOp &Op,
                         const AttributeList &FuncAttributes) const override {
    return LLT::scalar(8);
  }

  MachineBasicBlock *
  EmitInstrWithCustomInserter(MachineInstr &MI,
                              MachineBasicBlock *MBB) const override;
};

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802ISELLOWERING_H
