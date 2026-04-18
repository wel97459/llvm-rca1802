//===- RCA1802Disassembler.cpp - Disassembler for RCA1802 -----------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDecoder.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Endian.h"

using namespace llvm;

#define DEBUG_TYPE "rca1802-disassembler"

using DecodeStatus = MCDisassembler::DecodeStatus;

namespace {
class RCA1802Disassembler : public MCDisassembler {
  std::unique_ptr<const MCInstrInfo> const MII;
public:
  RCA1802Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx,
                      const MCInstrInfo *MII)
      : MCDisassembler(STI, Ctx), MII(MII) {}
  ~RCA1802Disassembler() override = default;

  DecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};
} // end anonymous namespace

static DecodeStatus DecodeGPR16RegisterClass(MCInst &Inst, unsigned RegNo,
                                         uint64_t Address,
                                         const void *Decoder) {
  if (RegNo > 15)
    return MCDisassembler::Fail;
  Inst.addOperand(MCOperand::createReg(RCA1802::R0 + RegNo));
  return MCDisassembler::Success;
}

static DecodeStatus decodeImm8Operand(MCInst &Inst, uint64_t Imm,
                                     uint64_t Address,
                                     const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus decodeAddr8Operand(MCInst &Inst, uint64_t Imm,
                                      uint64_t Address,
                                      const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus decodeAddr16Operand(MCInst &Inst, uint64_t Imm,
                                       uint64_t Address,
                                       const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

namespace llvm {
namespace MCD {
template <typename T> constexpr uint32_t InsnBitWidth = 0;
template <> constexpr uint32_t InsnBitWidth<uint64_t> = 64;
}
}

using namespace llvm::MCD;

#include "RCA1802GenDisassemblerTables.inc"

DecodeStatus RCA1802Disassembler::getInstruction(MCInst &Instr, uint64_t &Size,
                                             ArrayRef<uint8_t> Bytes,
                                             uint64_t Address,
                                             raw_ostream &CStream) const {
  Size = 0;
  if (Bytes.empty()) {
    return Fail;
  }

  uint64_t Insn = 0;
  size_t MaxSize = std::min<size_t>(Bytes.size(), 4);
  for (size_t i = 0; i < MaxSize; ++i) {
    Insn = (Insn << 8) | Bytes[i];
  }

  DecodeStatus Result = decodeInstruction(DecoderTableRCA18028, Instr, Insn, Address, this, STI);
  if (Result != Fail) {
    Size = MII->get(Instr.getOpcode()).getSize();
    if (Size == 0) Size = 1; 
    return Result;
  }

  return Fail;
}

static MCDisassembler *createRCA1802Disassembler(const Target &T,
                                             const MCSubtargetInfo &STI,
                                             MCContext &Ctx) {
  return new RCA1802Disassembler(STI, Ctx, T.createMCInstrInfo());
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRCA1802Disassembler() {
  TargetRegistry::RegisterMCDisassembler(getTheRCA1802Target(),
                                         createRCA1802Disassembler);
}
