//===-- RCA1802StaticStackAlloc.h - RCA1802 Static Stack Allocation -----*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 static stack allocation pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802STATICSTACKALLOC_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802STATICSTACKALLOC_H

#include "llvm/Pass.h"

namespace llvm {

ModulePass *createRCA1802StaticStackAllocPass();

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802STATICSTACKALLOC_H
