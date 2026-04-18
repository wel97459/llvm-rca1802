//===-- RCA1802Internalize.h - RCA1802 Libcall Internalization ----------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.n with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 library call internalization pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INTERNALIZE_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INTERNALIZE_H

namespace llvm {

class ModulePass;

ModulePass *createRCA1802InternalizePass();

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802INTERNALIZE_H
