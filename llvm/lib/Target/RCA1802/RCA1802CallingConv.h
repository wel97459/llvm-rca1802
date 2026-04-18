//===-- RCA1802CallingConv.h - RCA1802 Calling Conventions ------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 calling conventions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802CALLINGCONV_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802CALLINGCONV_H

#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/MC/MCRegisterInfo.h"

namespace llvm {

bool CC_RCA1802(unsigned ValNo, MVT ValVT, MVT LocVT, CCValAssign::LocInfo LocInfo,
                ISD::ArgFlagsTy ArgFlags, Type *OrigTy, CCState &State);

bool CC_RCA1802_VarArgs(unsigned ValNo, MVT ValVT, MVT LocVT,
                        CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                        Type *OrigTy, CCState &State);

} // namespace llvm

#endif // LLVM_LIB_TARGET_RCA1802_RCA1802CALLINGCONV_H
