//===-- RCA1802ZeroPageAlloc.h - RCA1802 Zero Page Allocation -----------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file This file declares the RCA1802 zero page allocation pass.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802ZEROPAGEALLOC_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802ZEROPAGEALLOC_H

#include "llvm/Pass.h"

namespace llvm {

ModulePass *createRCA1802ZeroPageAllocPass();

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802ZEROPAGEALLOC_H
