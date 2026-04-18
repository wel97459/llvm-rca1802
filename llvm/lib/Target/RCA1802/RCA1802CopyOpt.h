//===-- RCA1802CopyOpt.h - RCA1802 Copy Optimization -----------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802COPYOPT_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802COPYOPT_H

#include "llvm/CodeGen/MachineFunctionPass.h"

namespace llvm {

MachineFunctionPass *createRCA1802CopyOptPass();

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RCA1802_RCA1802COPYOPT_H
