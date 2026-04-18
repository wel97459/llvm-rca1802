//===-- RCA1802TargetStreamer.cpp - RCA1802 Target Streamer -----------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802TargetStreamer.h"
#include "MCTargetDesc/RCA1802MCELFStreamer.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

RCA1802TargetStreamer::RCA1802TargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

void RCA1802TargetStreamer::finish() {}

void RCA1802TargetStreamer::stronglyReference(StringRef Name, StringRef Comment) {}

void RCA1802TargetAsmStreamer::changeSection(const MCSection *CurSection, MCSection *Section,
                                         uint32_t SubSection, raw_ostream &OS) {}

void RCA1802TargetAsmStreamer::stronglyReference(MCSymbol *Sym) {}

RCA1802TargetELFStreamer::RCA1802TargetELFStreamer(MCStreamer &S, const MCSubtargetInfo &STI)
    : RCA1802TargetStreamer(S) {}

bool RCA1802TargetELFStreamer::emitDirectiveZeroPage(MCSymbol *Symbol) { return true; }

bool RCA1802TargetELFStreamer::hasBSS() { return false; }
bool RCA1802TargetELFStreamer::hasZPBSS() { return false; }
bool RCA1802TargetELFStreamer::hasData() { return false; }
bool RCA1802TargetELFStreamer::hasZPData() { return false; }
bool RCA1802TargetELFStreamer::hasInitArray() { return false; }
bool RCA1802TargetELFStreamer::hasFiniArray() { return false; }

void RCA1802TargetELFStreamer::stronglyReference(MCSymbol *Sym) {}

} // namespace llvm
