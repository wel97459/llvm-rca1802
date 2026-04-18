//===-- RCA1802MCExpr.cpp - RCA1802 specific MC expression classes ----------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802MCExpr.h"
#include "RCA1802FixupKinds.h"
#include "RCA1802ModifierNames.h"

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCValue.h"

namespace llvm {

const RCA1802MCExpr *RCA1802MCExpr::create(VariantKind Kind, const MCExpr *Expr,
                                   bool Negated, MCContext &Ctx) {
  return new (Ctx) RCA1802MCExpr(Kind, Expr, Negated);
}

void RCA1802MCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  assert(Kind != VK_NONE);

  if (isNegated()) {
    OS << '-';
  }

  OS << getName() << '(';
  MAI->printExpr(OS, *getSubExpr());
  OS << ')';
}

bool RCA1802MCExpr::evaluateAsConstant(int64_t &Result) const {
  MCValue Value;

  bool IsRelocatable = getSubExpr()->evaluateAsRelocatable(Value, nullptr);

  if (!IsRelocatable) {
    return false;
  }

  if (Value.isAbsolute()) {
    Result = evaluateAsInt64(Value.getConstant());
    return true;
  }

  return false;
}

bool RCA1802MCExpr::evaluateAsRelocatableImpl(MCValue &Result,
                                          const MCAssembler *Asm) const {
  if (!SubExpr->evaluateAsRelocatable(Result, Asm))
    return false;

  // If the subexpression is an absolute constant, apply the variant transform
  // here so generic directive range checks (e.g. .byte) see the masked value.
  if (Result.isAbsolute()) {
    int64_t V = Result.getConstant();
    V = evaluateAsInt64(V);
    Result = MCValue::get(V);
  }

  return true;
}

int64_t RCA1802MCExpr::evaluateAsInt64(int64_t Value) const {
  if (Negated) {
    Value *= -1;
  }

  switch (Kind) {
  case RCA1802MCExpr::VK_IMM8:
  case RCA1802MCExpr::VK_ADDR8:
  case RCA1802MCExpr::VK_ADDR16_LO:
  case RCA1802MCExpr::VK_ADDR24_SEGMENT_LO:
    Value &= 0xff;
    break;
  case RCA1802MCExpr::VK_ADDR16_HI:
  case RCA1802MCExpr::VK_ADDR24_SEGMENT_HI:
    Value &= 0xff00;
    Value >>= 8;
    break;
  case RCA1802MCExpr::VK_ADDR24_BANK:
    Value &= 0xff0000;
    Value >>= 16;
    break;
  case RCA1802MCExpr::VK_IMM16:
  case RCA1802MCExpr::VK_ADDR16:
  case RCA1802MCExpr::VK_ADDR24_SEGMENT:
    Value &= 0xffff;
    break;
  case RCA1802MCExpr::VK_ADDR24:
    Value &= 0xffffff;
    break;
  case RCA1802MCExpr::VK_ADDR13:
    Value &= 0x1fff;
    break;

  case RCA1802MCExpr::VK_ADDR_ASCIZ:
    llvm_unreachable("Unable to evaluate VK_ADDR_ASCIZ as int64.");

  case RCA1802MCExpr::VK_NONE:
    llvm_unreachable("Uninitialized expression.");
  }
  return static_cast<uint64_t>(Value);
}

RCA1802::Fixups RCA1802MCExpr::getFixupKind() const {
  RCA1802::Fixups Kind = RCA1802::Fixups::LastTargetFixupKind;

  switch (getKind()) {
  case VK_IMM8:
    Kind = RCA1802::Imm8;
    break;
  case VK_IMM16:
    Kind = RCA1802::Imm16;
    break;
  case VK_ADDR8:
    Kind = RCA1802::Addr8;
    break;
  case VK_ADDR16:
    Kind = RCA1802::Addr16;
    break;
  case VK_ADDR16_HI:
    Kind = RCA1802::Addr16_High;
    break;
  case VK_ADDR16_LO:
    Kind = RCA1802::Addr16_Low;
    break;
  case VK_ADDR24:
    Kind = RCA1802::Addr24;
    break;
  case VK_ADDR24_BANK:
    Kind = RCA1802::Addr24_Bank;
    break;
  case VK_ADDR24_SEGMENT:
    Kind = RCA1802::Addr24_Segment;
    break;
  case VK_ADDR24_SEGMENT_HI:
    Kind = RCA1802::Addr24_Segment_High;
    break;
  case VK_ADDR24_SEGMENT_LO:
    Kind = RCA1802::Addr24_Segment_Low;
    break;
  case VK_ADDR13:
    Kind = RCA1802::Addr13;
    break;
  case VK_ADDR_ASCIZ:
    Kind = RCA1802::AddrAsciz;
    break;
  case VK_NONE:
    llvm_unreachable("Uninitialized expression");
  }

  return Kind;
}

void RCA1802MCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

const char *RCA1802MCExpr::getName() const {
  const auto &Modifier = llvm::find_if(RCA1802::modifierNames(),
                                       [this](RCA1802::ModifierEntry const &Mod) {
                                         return Mod.VariantKind == Kind;
                                       });

  if (Modifier != std::end(RCA1802::modifierNames())) {
    return Modifier->Spelling;
  }
  return nullptr;
}

RCA1802MCExpr::VariantKind RCA1802MCExpr::getKindByName(StringRef Name,
                                                bool IsImmediate) {
  const auto &Modifier =
      llvm::find_if(RCA1802::modifierNames(),
                    [&Name, IsImmediate](RCA1802::ModifierEntry const &Mod) {
                      if (Mod.ImmediateOnly && !IsImmediate)
                        return false;
                      return Mod.Spelling == Name;
                    });

  if (Modifier != std::end(RCA1802::modifierNames())) {
    return Modifier->VariantKind;
  }
  return VK_NONE;
}

} // end of namespace llvm
