//===-- RCA1802AsmBackend.cpp - RCA1802 Asm Backend  ------------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RCA1802AsmBackend.h"
#include "MCTargetDesc/RCA1802ELFObjectWriter.h"
#include "MCTargetDesc/RCA1802FixupKinds.h"
#include "MCTargetDesc/RCA1802MCExpr.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802Subtarget.h"

#include "llvm/ADT/StringExtras.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

MCAsmBackend *createRCA1802AsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                  const MCRegisterInfo &MRI,
                                  const llvm::MCTargetOptions &TO) {
  return new RCA1802AsmBackend(STI.getTargetTriple().getOS());
}

std::unique_ptr<MCObjectTargetWriter>
RCA1802AsmBackend::createObjectTargetWriter() const {
  return createRCA1802ELFObjectWriter(MCELFObjectTargetWriter::getOSABI(OSType));
}

bool RCA1802AsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup,
                                         uint64_t Value) const {
  return false;
}

bool RCA1802AsmBackend::fixupNeedsRelaxationAdvanced(const MCFragment &F,
                                                 const MCFixup &Fixup,
                                                 const MCValue &Target,
                                                 uint64_t Value,
                                                 bool Resolved) const {
  return false;
}

MCFixupKindInfo RCA1802AsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  if (Kind < FirstTargetFixupKind) {
    return MCAsmBackend::getFixupKindInfo(Kind);
  }

  return RCA1802FixupKinds::getFixupKindInfo(static_cast<RCA1802::Fixups>(Kind), this);
}

void RCA1802AsmBackend::applyFixup(const MCFragment &F, const MCFixup &Fixup,
                               const MCValue &Target, uint8_t *Data,
                               uint64_t Value, bool IsResolved) {
  unsigned int Kind = Fixup.getKind();
  unsigned int Bytes = 0;

  switch (Kind) {
  case RCA1802::Imm8:
  case RCA1802::Addr8:
  case FK_Data_1:
    Bytes = 1;
    break;
  case RCA1802::Imm16:
  case RCA1802::Addr16:
  case FK_Data_2:
    Bytes = 2;
    break;
  case RCA1802::Addr24:
  case FK_Data_4:
    Bytes = 4;
    break;
  default:
    return;
  }

  for (unsigned i = 0; i != Bytes; ++i) {
    unsigned Idx = Bytes - 1 - i;
    Data[Idx] = uint8_t((Value >> (i * 8)) & 0xff);
  }
}

bool RCA1802AsmBackend::mayNeedRelaxation(unsigned Opcode,
                                      ArrayRef<MCOperand> Operands,
                                      const MCSubtargetInfo &STI) const {
  return false;
}

void RCA1802AsmBackend::relaxInstruction(MCInst &Inst,
                                     const MCSubtargetInfo &STI) const {
}

unsigned RCA1802AsmBackend::relaxInstructionTo(unsigned Opcode,
                                           const MCSubtargetInfo &STI,
                                           bool &BankRelax) {
  return 0;
}

void RCA1802AsmBackend::translateOpcodeToSubtarget(MCInst &Inst,
                                               const MCSubtargetInfo &STI) {
}

void RCA1802AsmBackend::relaxForImmediate(MCInst &Inst, const MCSubtargetInfo &STI) {
}

bool RCA1802AsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                 const MCSubtargetInfo *STI) const {
  while (Count--) {
    OS << '\xc4'; // RCA1802 NOP
  }
  return true;
}

} // namespace llvm
