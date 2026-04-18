//===-- RCA1802CallLowering.cpp - Call lowering ---------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the lowering of LLVM calls to Machine Instructions.
//
//===----------------------------------------------------------------------===//

#include "RCA1802CallLowering.h"
#include "RCA1802CallingConv.h"
#include "RCA1802ISelLowering.h"
#include "RCA1802MachineFunctionInfo.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/Analysis.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

using namespace llvm;

namespace {

struct RCA1802OutgoingValueHandler : public CallLowering::OutgoingValueHandler {
  RCA1802OutgoingValueHandler(MachineIRBuilder &MIRBuilder,
                          MachineRegisterInfo &MRI, MachineInstrBuilder &MIB)
      : OutgoingValueHandler(MIRBuilder, MRI), MIB(MIB) {}

  void assignValueToReg(Register ValVReg, Register PhysReg,
                        const CCValAssign &VA, ISD::ArgFlagsTy Flags) override {
    MIRBuilder.buildCopy(PhysReg, ValVReg);
    MIB.addUse(PhysReg, RegState::Implicit);
  }

  void assignValueToAddress(Register ValVReg, Register AddrReg, LLT MemTy,
                            const MachinePointerInfo &MPO,
                            const CCValAssign &VA) override {
    MachineFunction &MF = MIRBuilder.getMF();
    auto *MMO = MF.getMachineMemOperand(MPO, MachineMemOperand::MOStore, MemTy,
                                        inferAlignFromPtrInfo(MF, MPO));
    MIRBuilder.buildStore(ValVReg, AddrReg, *MMO);
  }

  Register getStackAddress(uint64_t Size, int64_t Offset,
                           MachinePointerInfo &MPO,
                           ISD::ArgFlagsTy Flags) override {
    MachineFunction &MF = MIRBuilder.getMF();
    int FI = MF.getFrameInfo().CreateFixedObject(Size, Offset, true);
    MPO = MachinePointerInfo::getFixedStack(MF, FI);
    auto AddrReg = MIRBuilder.buildFrameIndex(LLT::pointer(0, 16), FI);
    return AddrReg.getReg(0);
  }

  MachineInstrBuilder &MIB;
};

struct RCA1802OutgoingArgsHandler : public RCA1802OutgoingValueHandler {
  RCA1802OutgoingArgsHandler(MachineIRBuilder &MIRBuilder,
                         MachineRegisterInfo &MRI, MachineInstrBuilder &MIB)
      : RCA1802OutgoingValueHandler(MIRBuilder, MRI, MIB) {}

  Register getStackAddress(uint64_t Size, int64_t Offset,
                           MachinePointerInfo &MPO,
                           ISD::ArgFlagsTy Flags) override {
    LLT P = LLT::pointer(0, 16);
    Register SPReg;
    SPReg = MIRBuilder.buildCopy(P, Register(RCA1802::R2)).getReg(0);
    auto OffsetReg = MIRBuilder.buildConstant(LLT::scalar(16), Offset);
    auto AddrReg = MIRBuilder.buildPtrAdd(P, SPReg, OffsetReg);
    MPO = MachinePointerInfo::getStack(MIRBuilder.getMF(), Offset);
    return AddrReg.getReg(0);
  }
};

struct RCA1802IncomingValueHandler : public CallLowering::IncomingValueHandler {
  RCA1802IncomingValueHandler(MachineIRBuilder &MIRBuilder,
                          MachineRegisterInfo &MRI)
      : IncomingValueHandler(MIRBuilder, MRI) {}

  void assignValueToReg(Register ValVReg, Register PhysReg,
                        const CCValAssign &VA, ISD::ArgFlagsTy Flags) override {
    MIRBuilder.buildCopy(ValVReg, PhysReg);
  }

  void assignValueToAddress(Register ValVReg, Register AddrReg, LLT MemTy,
                            const MachinePointerInfo &MPO,
                            const CCValAssign &VA) override {
    MachineFunction &MF = MIRBuilder.getMF();
    auto *MMO = MF.getMachineMemOperand(MPO, MachineMemOperand::MOLoad, MemTy,
                                        inferAlignFromPtrInfo(MF, MPO));
    MIRBuilder.buildLoad(ValVReg, AddrReg, *MMO);
  }

  Register getStackAddress(uint64_t Size, int64_t Offset,
                           MachinePointerInfo &MPO,
                           ISD::ArgFlagsTy Flags) override {
    MachineFunction &MF = MIRBuilder.getMF();
    int FI = MF.getFrameInfo().CreateFixedObject(Size, Offset, true);
    MPO = MachinePointerInfo::getFixedStack(MF, FI);
    auto AddrReg = MIRBuilder.buildFrameIndex(LLT::pointer(0, 16), FI);
    return AddrReg.getReg(0);
  }
};

struct RCA1802IncomingReturnHandler : public RCA1802IncomingValueHandler {
  RCA1802IncomingReturnHandler(MachineIRBuilder &MIRBuilder,
                           MachineRegisterInfo &MRI)
      : RCA1802IncomingValueHandler(MIRBuilder, MRI) {}

  Register getStackAddress(uint64_t Size, int64_t Offset,
                           MachinePointerInfo &MPO,
                           ISD::ArgFlagsTy Flags) override {
    LLT P = LLT::pointer(0, 16);
    Register SPReg;
    SPReg = MIRBuilder.buildCopy(P, Register(RCA1802::R2)).getReg(0);
    auto OffsetReg = MIRBuilder.buildConstant(LLT::scalar(16), Offset);
    auto AddrReg = MIRBuilder.buildPtrAdd(P, SPReg, OffsetReg);
    MPO = MachinePointerInfo::getStack(MIRBuilder.getMF(), Offset);
    return AddrReg.getReg(0);
  }
};

} // namespace

bool RCA1802CallLowering::lowerReturn(MachineIRBuilder &MIRBuilder,
                                  const Value *Val, ArrayRef<Register> VRegs,
                                  FunctionLoweringInfo &FLI) const {
  MachineFunction &MF = MIRBuilder.getMF();
  auto MIB = MIRBuilder.buildInstrNoInsert(RCA1802::RET);

  if (!VRegs.empty()) {
    MachineRegisterInfo &MRI = MF.getRegInfo();
    const DataLayout &DL = MF.getDataLayout();
    const RCA1802TargetLowering &TLI = *getTLI<RCA1802TargetLowering>();

    SmallVector<ArgInfo, 8> SplitArgs;
    ArgInfo OrigArg{VRegs, Val->getType(), 0};
    setArgFlags(OrigArg, AttributeList::ReturnIndex, DL, *FLI.Fn);
    CallLowering::splitToValueTypes(OrigArg, SplitArgs, DL, TLI.getCallingConv());

    RCA1802OutgoingValueHandler Handler(MIRBuilder, MRI, MIB);
    OutgoingValueAssigner Assigner(llvm::CC_RCA1802);
    if (!determineAndHandleAssignments(Handler, Assigner, SplitArgs, MIRBuilder, TLI.getCallingConv(), false))
      return false;
  }

  MIRBuilder.insertInstr(MIB);
  return true;
}

bool RCA1802CallLowering::lowerFormalArguments(
    MachineIRBuilder &MIRBuilder, const Function &F,
    ArrayRef<ArrayRef<Register>> VRegs, FunctionLoweringInfo &FLI) const {
  MachineFunction &MF = MIRBuilder.getMF();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const DataLayout &DL = MF.getDataLayout();
  const RCA1802TargetLowering &TLI = *getTLI<RCA1802TargetLowering>();

  SmallVector<ArgInfo, 8> SplitArgs;
  unsigned i = 0;
  for (const auto &Arg : F.args()) {
    ArgInfo OrigArg{VRegs[i], Arg.getType(), i};
    setArgFlags(OrigArg, i + AttributeList::FirstArgIndex, DL, F);
    CallLowering::splitToValueTypes(OrigArg, SplitArgs, DL, TLI.getCallingConv());
    ++i;
  }

  RCA1802IncomingValueHandler Handler(MIRBuilder, MRI);
  IncomingValueAssigner Assigner(llvm::CC_RCA1802);
  if (!determineAndHandleAssignments(Handler, Assigner, SplitArgs, MIRBuilder, TLI.getCallingConv(), F.isVarArg()))
    return false;

  return true;
}

bool RCA1802CallLowering::lowerCall(MachineIRBuilder &MIRBuilder,
                               CallLoweringInfo &Info) const {
  MachineFunction &MF = MIRBuilder.getMF();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const DataLayout &DL = MF.getDataLayout();
  const RCA1802TargetLowering &TLI = *getTLI<RCA1802TargetLowering>();

  SmallVector<ArgInfo, 8> OutArgs;
  for (auto &Arg : Info.OrigArgs) {
    CallLowering::splitToValueTypes(Arg, OutArgs, DL, TLI.getCallingConv());
  }

  SmallVector<ArgInfo, 8> InArgs;
  if (!Info.CanLowerReturn) {
    CallLowering::splitToValueTypes(Info.OrigRet, InArgs, DL, TLI.getCallingConv());
  }

  // Calculate stack size required for arguments
  OutgoingValueAssigner Assigner(llvm::CC_RCA1802);
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(Info.CallConv, Info.IsVarArg, MF, ArgLocs, MF.getFunction().getContext());
  determineAssignments(Assigner, OutArgs, CCInfo);
  unsigned StackSize = CCInfo.getStackSize();

  auto CallSeqStart = MIRBuilder.buildInstr(RCA1802::ADJCALLSTACKDOWN);
  CallSeqStart.addImm(StackSize).addImm(0);

  auto Call = MIRBuilder.buildInstrNoInsert(RCA1802::CALL_inst)
                  .add(Info.Callee);

  RCA1802OutgoingArgsHandler OutHandler(MIRBuilder, MRI, Call);
  if (!handleAssignments(OutHandler, OutArgs, CCInfo, ArgLocs, MIRBuilder))
    return false;

  MIRBuilder.insertInstr(Call);

  if (!Info.CanLowerReturn) {
    RCA1802IncomingReturnHandler InHandler(MIRBuilder, MRI);
    IncomingValueAssigner InAssigner(llvm::CC_RCA1802);
    if (!determineAndHandleAssignments(InHandler, InAssigner, InArgs, MIRBuilder, Info.CallConv, Info.IsVarArg))
      return false;
  }

  MIRBuilder.buildInstr(RCA1802::ADJCALLSTACKUP).addImm(StackSize).addImm(0);

  return true;
}
