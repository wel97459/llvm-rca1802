//===-- RCA1802TargetInfo.cpp - RCA1802 Target Implementation ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/RCA1802TargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

namespace llvm {
Target &getTheRCA1802Target() {
  static Target TheRCA1802Target;
  return TheRCA1802Target;
}
} // namespace llvm

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRCA1802TargetInfo() { // NOLINT
  llvm::RegisterTarget<llvm::Triple::rca1802> X(llvm::getTheRCA1802Target(), "rca1802",
                                            "RCA1802 Technologies 65xx and variants", "RCA1802");
}

