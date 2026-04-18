//===-- RCA1802IndexIV.h - RCA1802 Index IV Pass ------------------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 Index IV pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INDEXIV_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INDEXIV_H

#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"

namespace llvm {

struct RCA1802IndexIV : public PassInfoMixin<RCA1802IndexIV> {
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR,
                        LPMUpdater &U);
};

} // end namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802INDEXIV_H
