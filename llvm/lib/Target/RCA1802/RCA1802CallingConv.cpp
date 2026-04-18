//===-- RCA1802CallingConv.cpp - Calling Conventions ------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the RCA1802 calling convention.
//
//===----------------------------------------------------------------------===//

#include "RCA1802CallingConv.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802RegisterInfo.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/MC/MCRegisterInfo.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/IR/DataLayout.h"

using namespace llvm;

namespace llvm {
#include "RCA1802GenCallingConv.inc"

bool CC_RCA1802(unsigned ValNo, MVT ValVT, MVT LocVT, CCValAssign::LocInfo LocInfo,
                ISD::ArgFlagsTy ArgFlags, Type *OrigTy, CCState &State) {
    return CC_RCA1802_Impl(ValNo, ValVT, LocVT, LocInfo, ArgFlags, OrigTy, State);
}

bool CC_RCA1802_VarArgs(unsigned ValNo, MVT ValVT, MVT LocVT,
                        CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                        Type *OrigTy, CCState &State) {
    return CC_RCA1802_VarArgs_Impl(ValNo, ValVT, LocVT, LocInfo, ArgFlags, OrigTy, State);
}
} // namespace llvm
