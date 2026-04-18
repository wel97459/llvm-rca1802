//===-- RCA1802LegalizerInfo.cpp ------------------------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802LegalizerInfo.h"
#include "RCA1802Subtarget.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/TargetLowering.h"

using namespace llvm;

RCA1802LegalizerInfo::RCA1802LegalizerInfo(const RCA1802Subtarget &STI) {
  using namespace TargetOpcode;

  const LLT s1 = LLT::scalar(1);
  const LLT s8 = LLT::scalar(8);
  const LLT s16 = LLT::scalar(16);
  const LLT p0 = LLT::pointer(0, 16);

  getActionDefinitionsBuilder(G_ADD)
    .legalFor({s8, s16})
    .clampScalar(0, s8, s16);

  getActionDefinitionsBuilder({G_LOAD, G_STORE})
    .legalForTypesWithMemDesc({{s8, p0, s8, 8}, {s16, p0, s16, 8}});

  getActionDefinitionsBuilder(G_CONSTANT)
    .legalFor({s8, s16});

  getActionDefinitionsBuilder(G_FRAME_INDEX)
    .legalFor({p0});

  getLegacyLegalizerInfo().computeTables();
}
