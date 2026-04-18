//===-- RCA1802TargetObjectFile.cpp - RCA1802 Object Info -------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802TargetObjectFile.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/IR/GlobalObject.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

void RCA1802TargetObjectFile::Initialize(MCContext &Ctx,
                                     const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
}

MCSection *RCA1802TargetObjectFile::SelectSectionForGlobal(
    const GlobalObject *GO, SectionKind Kind, const TargetMachine &TM) const {
  if (GO->getAddressSpace() == RCA1802::AS_ZeroPage && !GO->hasSection()) {
    if (Kind.isBSS())
      return getContext().getELFSection(".zp.bss", ELF::SHT_NOBITS,
                                        ELF::SHF_WRITE | ELF::SHF_ALLOC |
                                            ELF::SHF_RCA1802_ZEROPAGE);
    return getContext().getELFSection(".zp.data", ELF::SHT_PROGBITS,
                                      ELF::SHF_WRITE | ELF::SHF_ALLOC |
                                          ELF::SHF_RCA1802_ZEROPAGE);
  }

  return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);
}
