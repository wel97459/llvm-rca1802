//===-- RCA1802LateOptimization.h - RCA1802 Copy Insertion --------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 copy insertion pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INSERTCOPIES_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INSERTCOPIES_H

#include "llvm/CodeGen/MachineFunctionPass.h"

namespace llvm {

MachineFunctionPass *createRCA1802InsertCopiesPass();

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802INSERTCOPIES_H
