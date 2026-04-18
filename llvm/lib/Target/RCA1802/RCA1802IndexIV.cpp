//===-- RCA1802IndexIV.cpp ------------------------------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802IndexIV.h"
#include "RCA1802.h"
#include "RCA1802InstrInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

using namespace llvm;

PreservedAnalyses RCA1802IndexIV::run(Loop &L, LoopAnalysisManager &AM,
                                  LoopStandardAnalysisResults &AR,
                                  LPMUpdater &U) {
  const DataLayout &DL = L.getHeader()->getModule()->getDataLayout();
  bool Changed = false;
  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
