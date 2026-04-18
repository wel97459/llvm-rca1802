//===-- RCA1802ModifierNames.h - RCA1802 modifier name table ------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802MODIFIERNAMES_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802MODIFIERNAMES_H

#include "MCTargetDesc/RCA1802MCExpr.h"
#include "llvm/ADT/ArrayRef.h"

namespace llvm {
namespace RCA1802 {

struct ModifierEntry {
  const char *const Spelling;
  RCA1802MCExpr::VariantKind VariantKind;
  bool ImmediateOnly = false;
};

llvm::ArrayRef<ModifierEntry> modifierNames();

} // namespace RCA1802
} // namespace llvm

#endif // LLVM_LIB_TARGET_RCA1802_RCA1802MODIFIERNAMES_H
