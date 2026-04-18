//===-- RCA1802MCTargetDesc.h - RCA1802 Target Descriptions -------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RCA1802_MCTARGET_DESC_H
#define LLVM_RCA1802_MCTARGET_DESC_H

#include "llvm/ADT/Sequence.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;
class Triple;

Target &getTheRCA1802Target();

MCInstrInfo *createRCA1802MCInstrInfo();

MCCodeEmitter *createRCA1802MCCodeEmitter(const MCInstrInfo &MCII,
                                      MCContext &Ctx);

MCAsmBackend *createRCA1802AsmBackend(const Target &T,
                                  const MCSubtargetInfo &STI,
                                  const MCRegisterInfo &MRI,
                                  const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createRCA1802ELFObjectWriter(uint8_t OSABI);

} // end namespace llvm

#define GET_REGINFO_ENUM
#include "RCA1802GenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "RCA1802GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "RCA1802GenSubtargetInfo.inc"

namespace llvm {
namespace RCA1802Op {
enum OperandType : unsigned {
  OPERAND_IMM8 = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_ADDR8,
  OPERAND_ADDR16,
  OPERAND_IMM16,
  OPERAND_ADDR24,
  OPERAND_ADDR13
};
} // namespace RCA1802Op

namespace RCA1802 {
enum TSFlag {
  TSFlagMLow = (1 << 0),
  TSFlagMHigh = (1 << 1),
  TSFlagXLow = (1 << 2),
  TSFlagXHigh = (1 << 3)
};

enum AddressSpace { AS_Memory = 0, AS_ZeroPage = 1 };

bool isZeroPageSectionName(StringRef Name);
} // namespace RCA1802
} // namespace llvm

#endif // LLVM_RCA1802_MCTARGET_DESC_H
