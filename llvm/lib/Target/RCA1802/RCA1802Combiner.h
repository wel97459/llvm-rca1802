//===-- RCA1802Combiner.h - RCA1802 GlobalIsel Combiner -----------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 global machine instruction combiner.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802COMBINER_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802COMBINER_H

#include "llvm/Pass.h"

namespace llvm {

FunctionPass *createRCA1802Combiner();

} // end namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802COMBINER_H
