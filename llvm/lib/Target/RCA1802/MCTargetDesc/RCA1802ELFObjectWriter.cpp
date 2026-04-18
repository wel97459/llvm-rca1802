//===-- RCA1802ELFObjectWriter.cpp - RCA1802 ELF Writer ---------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RCA1802FixupKinds.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

class RCA1802ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  RCA1802ELFObjectWriter(uint8_t OSABI);

  ~RCA1802ELFObjectWriter() override = default;

protected:
  unsigned getRelocType(const MCFixup &Fixup, const MCValue &Target,
                        bool IsPCRel) const override;
};

RCA1802ELFObjectWriter::RCA1802ELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false, OSABI, ELF::EM_RCA1802, /*HasRelocationAddend=*/true) {}

unsigned RCA1802ELFObjectWriter::getRelocType(const MCFixup &Fixup,
                                          const MCValue &Target,
                                          bool IsPCRel) const {
  switch ((RCA1802::Fixups)Fixup.getKind()) {
  case RCA1802::Addr16:
    return ELF::R_RCA1802_ADDR16;
  case RCA1802::Imm8:
    return ELF::R_RCA1802_IMM8;
  default:
    return ELF::R_X86_64_NONE;
  }
}

std::unique_ptr<MCObjectTargetWriter> createRCA1802ELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<RCA1802ELFObjectWriter>(OSABI);
}

} // end namespace llvm
