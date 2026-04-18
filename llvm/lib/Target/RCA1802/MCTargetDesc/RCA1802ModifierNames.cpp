//===-- RCA1802ModifierNames.cpp - RCA1802 modifier name table --------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802ModifierNames.h"
#include "llvm/ADT/ArrayRef.h"

namespace llvm {
namespace RCA1802 {

static const struct ModifierEntry ModifierNames[] = {
    // Define immediate variants of rca18028() and rca180216() first.
    {"rca18028", RCA1802MCExpr::VK_IMM8, true},
    {"rca180216", RCA1802MCExpr::VK_IMM16, true},
    {"rca18028", RCA1802MCExpr::VK_ADDR8, false},
    {"rca180216", RCA1802MCExpr::VK_ADDR16, false},
    {"rca180216lo", RCA1802MCExpr::VK_ADDR16_LO, false},
    {"rca180216hi", RCA1802MCExpr::VK_ADDR16_HI, false},
    {"rca180224", RCA1802MCExpr::VK_ADDR24, false},
    {"rca180224bank", RCA1802MCExpr::VK_ADDR24_BANK, false},
    {"rca180224segment", RCA1802MCExpr::VK_ADDR24_SEGMENT, false},
    {"rca180224segmentlo", RCA1802MCExpr::VK_ADDR24_SEGMENT_LO, false},
    {"rca180224segmenthi", RCA1802MCExpr::VK_ADDR24_SEGMENT_HI, false},
    {"rca180213", RCA1802MCExpr::VK_ADDR13, false},
};

llvm::ArrayRef<ModifierEntry> modifierNames() { return ModifierNames; }

} // namespace RCA1802
} // namespace llvm
