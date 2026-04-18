//===-- RCA1802MCAsmInfo.cpp - RCA1802 asm properties -----------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the RCA1802MCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "RCA1802MCAsmInfo.h"
#include "MCTargetDesc/RCA1802MCExpr.h"
#include "RCA1802MCTargetDesc.h"

#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {

static const MCAsmInfo::AtSpecifier AtSpecifiers[] = {
    {RCA1802MCExpr::VK_IMM8, "rca1802imm8"},
    {RCA1802MCExpr::VK_IMM16, "rca1802imm16"},
    {RCA1802MCExpr::VK_ADDR8, "rca18028"},
    {RCA1802MCExpr::VK_ADDR16, "rca180216"},
    {RCA1802MCExpr::VK_ADDR16_LO, "rca180216lo"},
    {RCA1802MCExpr::VK_ADDR16_HI, "rca180216hi"},
    {RCA1802MCExpr::VK_ADDR24, "rca180224"},
    {RCA1802MCExpr::VK_ADDR24_BANK, "rca180224bank"},
    {RCA1802MCExpr::VK_ADDR24_SEGMENT, "rca180224segment"},
    {RCA1802MCExpr::VK_ADDR24_SEGMENT_LO, "rca180224segmentlo"},
    {RCA1802MCExpr::VK_ADDR24_SEGMENT_HI, "rca180224segmenthi"},
    {RCA1802MCExpr::VK_ADDR13, "rca180213"},
};

RCA1802MCAsmInfo::RCA1802MCAsmInfo(const Triple &TT, const MCTargetOptions &Options) {
  // While the platform uses 2-byte pointers, the ELF files use 4-byte pointers
  // to convey banking information; this field is used, among others, by the
  // DWARF debug structures.
  CodePointerSize = 4;
  CalleeSaveStackSlotSize = 0;
  SeparatorString = "\n";
  CommentString = ";";
  // RCA1802 is big-endian
  IsLittleEndian = false;
  // Maximum instruction length.
  MaxInstLength = 3;
  SupportsDebugInformation = true;

  initializeAtSpecifiers(AtSpecifiers);
}

unsigned RCA1802MCAsmInfo::getMaxInstLength(const MCSubtargetInfo *STI) const {
  return 3;
}

} //  namespace llvm
