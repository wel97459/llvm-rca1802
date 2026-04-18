//===-- RCA1802LateOptimization.h - RCA1802 Late Optimizationn ----------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 late optimization pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802LATEOPTIMIZATION_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802LATEOPTIMIZATION_H

#include "llvm/CodeGen/MachineFunctionPass.h"

namespace llvm {

MachineFunctionPass *createRCA1802LateOptimizationPass();

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802LATEOPTIMIZATION_H
