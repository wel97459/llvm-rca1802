//===--------- RCA1802MCELFStreamer.h - RCA1802 subclass of MCELFStreamer ---------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_MCTARGETDESC_RCA1802MCELFSTREAMER_H
#define LLVM_LIB_TARGET_RCA1802_MCTARGETDESC_RCA1802MCELFSTREAMER_H

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include <memory>

namespace llvm {

class RCA1802MCELFStreamer : public MCELFStreamer {
  std::unique_ptr<const MCInstrInfo> const MCII;

public:
  RCA1802MCELFStreamer(MCContext &Context, std::unique_ptr<MCAsmBackend> TAB,
                   std::unique_ptr<MCObjectWriter> OW,
                   std::unique_ptr<MCCodeEmitter> Emitter);

  void initSections(bool NoExecStack, const MCSubtargetInfo &STI) override;
};

MCStreamer *createRCA1802MCELFStreamer(const Triple &T, MCContext &Ctx,
                                   std::unique_ptr<MCAsmBackend> &&TAB,
                                   std::unique_ptr<MCObjectWriter> &&OW,
                                   std::unique_ptr<MCCodeEmitter> &&Emitter);

} // end namespace llvm

#endif // LLVM_LIB_TARGET_RCA1802_MCTARGETDESC_RCA1802MCELFSTREAMER_H
