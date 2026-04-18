//===-- RCA1802AsmBackend.h - RCA1802 Asm Backend  --------------------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RCA1802_ASM_BACKEND_H
#define LLVM_RCA1802_ASM_BACKEND_H

#include "MCTargetDesc/RCA1802FixupKinds.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {

class RCA1802AsmBackend : public MCAsmBackend {
public:
  RCA1802AsmBackend(Triple::OSType OSType)
      : llvm::MCAsmBackend(endianness::big), OSType(OSType) {}

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  bool fixupNeedsRelaxation(const MCFixup &Fixup,
                            uint64_t Value) const override;
  bool fixupNeedsRelaxationAdvanced(const MCFragment &, const MCFixup &Fixup,
                                    const MCValue &Target, uint64_t Value,
                                    bool Resolved) const override;
  MCFixupKindInfo getFixupKindInfo(MCFixupKind Kind) const override;

  void applyFixup(const MCFragment &F, const MCFixup &Fixup,
                  const MCValue &Target, uint8_t *Data, uint64_t Value,
                  bool IsResolved) override;

  bool mayNeedRelaxation(unsigned Opcode, ArrayRef<MCOperand> Operands,
                         const MCSubtargetInfo &STI) const override;

  void relaxInstruction(MCInst &Inst,
                        const MCSubtargetInfo &STI) const override;

  static unsigned relaxInstructionTo(unsigned Opcode,
                                     const MCSubtargetInfo &STI,
                                     bool &BankRelax);
  static unsigned relaxInstructionTo(unsigned Opcode,
                                     const MCSubtargetInfo &STI) {
    bool BankRelax = false;
    return relaxInstructionTo(Opcode, STI, BankRelax);
  }

  static void translateOpcodeToSubtarget(MCInst &Inst,
                                         const MCSubtargetInfo &STI);

  static void relaxForImmediate(MCInst &Inst, const MCSubtargetInfo &STI);

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;

private:
  Triple::OSType OSType;
};

} // end namespace llvm

#endif // LLVM_RCA1802_ASM_BACKEND_H
