//===--------- RCA1802MCELFStreamer.cpp - RCA1802 subclass of MCELFStreamer -------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RCA1802MCELFStreamer.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

RCA1802MCELFStreamer::RCA1802MCELFStreamer(MCContext &Ctx,
                                       std::unique_ptr<MCAsmBackend> TAB,
                                       std::unique_ptr<MCObjectWriter> OW,
                                       std::unique_ptr<MCCodeEmitter> Emitter)
    : MCELFStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter)),
      MCII(createRCA1802MCInstrInfo()) {}

void RCA1802MCELFStreamer::initSections(bool NoExecStack,
                                    const MCSubtargetInfo &STI) {
  MCELFStreamer::initSections(NoExecStack, STI);
}

MCStreamer *createRCA1802MCELFStreamer(const Triple &T, MCContext &Ctx,
                                   std::unique_ptr<MCAsmBackend> &&TAB,
                                   std::unique_ptr<MCObjectWriter> &&OW,
                                   std::unique_ptr<MCCodeEmitter> &&Emitter) {
  return new RCA1802MCELFStreamer(Ctx, std::move(TAB), std::move(OW),
                                std::move(Emitter));
}

} // end namespace llvm
