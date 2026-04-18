//===-- RCA1802NonReentrant.h - RCA1802 NonReentrant Pass ---------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 NonReentrant pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802NORECURSE_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802NORECURSE_H

#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

ModulePass *createRCA1802NonReentrantPass();

struct RCA1802NonReentrantPass : PassInfoMixin<RCA1802NonReentrantPass> {
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};

} // end namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802NORECURSE_H
