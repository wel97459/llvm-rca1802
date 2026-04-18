//===-- RCA1802ELFObjectWriter.cpp - RCA1802 ELF Writer ---------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RCA1802FixupKinds.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"

#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

/// Writes RCA1802 machine code into an ELF32 object file.
class RCA1802ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit RCA1802ELFObjectWriter(uint8_t OSABI);
  virtual ~RCA1802ELFObjectWriter() = default;
  unsigned getRelocType(const MCFixup &Fixup, const MCValue &Target,
                        bool IsPCRel) const override;
};

} // end of namespace llvm
