//===-- RCA1802LegalizerInfo.h ----------------------------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the targeting of the LegalizerInfo class for RCA1802.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802LEGALIZERINFO_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802LEGALIZERINFO_H

#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"

namespace llvm {

class RCA1802Subtarget;

class RCA1802LegalizerInfo : public LegalizerInfo {
public:
  RCA1802LegalizerInfo(const RCA1802Subtarget &STI);
  virtual ~RCA1802LegalizerInfo() = default;
};

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802LEGALIZERINFO_H
