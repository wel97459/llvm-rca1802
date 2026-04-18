//===-- RCA1802AsmParser.cpp - Parse RCA1802 assembly to MCInst -----------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Casting.h"

using namespace llvm;

#define GET_REGISTER_MATCHER
#include "RCA1802GenAsmMatcher.inc"

namespace {

struct RCA1802Operand : public MCParsedAsmOperand {
  enum KindTy { Token, Register, Immediate } Kind;
  SMLoc StartLoc, EndLoc;

  union {
    struct {
      const char *Data;
      unsigned Length;
    } Tok;
    MCRegister Reg;
    const MCExpr *Imm;
  };

  explicit RCA1802Operand(KindTy K) : Kind(K) {}

  bool isToken() const override { return Kind == Token; }
  bool isReg() const override { return Kind == Register; }
  bool isImm() const override { return Kind == Immediate; }
  bool isMem() const override { return false; }

  SMLoc getStartLoc() const override { return StartLoc; }
  SMLoc getEndLoc() const override { return EndLoc; }

  StringRef getToken() const {
    assert(Kind == Token && "Invalid access!");
    return StringRef(Tok.Data, Tok.Length);
  }

  MCRegister getReg() const override {
    assert(Kind == Register && "Invalid access!");
    return Reg;
  }

  const MCExpr *getImm() const {
    assert(Kind == Immediate && "Invalid access!");
    return Imm;
  }

  void print(raw_ostream &OS, const MCAsmInfo &MAI) const override {
    switch (Kind) {
    case Token:
      OS << "Token: " << getToken();
      break;
    case Register:
      OS << "Reg: " << Reg;
      break;
    case Immediate:
      OS << "Imm: ";
      MAI.printExpr(OS, *Imm);
      break;
    }
  }

  void addRegOperands(MCInst &Inst, unsigned N) const {
    assert(Kind == Register && "Invalid access!");
    assert(N == 1 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(Reg));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    assert(Kind == Immediate && "Invalid access!");
    assert(N == 1 && "Invalid number of operands!");
    if (auto *CE = dyn_cast<MCConstantExpr>(Imm))
      Inst.addOperand(MCOperand::createImm(CE->getValue()));
    else
      Inst.addOperand(MCOperand::createExpr(Imm));
  }

  static std::unique_ptr<RCA1802Operand> createToken(StringRef Str, SMLoc S) {
    auto Op = std::make_unique<RCA1802Operand>(Token);
    Op->Tok.Data = Str.data();
    Op->Tok.Length = Str.size();
    Op->StartLoc = S;
    Op->EndLoc = S;
    return Op;
  }

  static std::unique_ptr<RCA1802Operand> createReg(MCRegister Reg, SMLoc S,
                                               SMLoc E) {
    auto Op = std::make_unique<RCA1802Operand>(Register);
    Op->Reg = Reg;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<RCA1802Operand> createImm(const MCExpr *Val, SMLoc S,
                                               SMLoc E) {
    auto Op = std::make_unique<RCA1802Operand>(Immediate);
    Op->Imm = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }
};

class RCA1802AsmParser : public MCTargetAsmParser {
#define GET_ASSEMBLER_HEADER
#include "RCA1802GenAsmMatcher.inc"

  bool parseRegister(MCRegister &Reg, SMLoc &StartLoc, SMLoc &EndLoc) override;
  ParseStatus tryParseRegister(MCRegister &Reg, SMLoc &StartLoc,
                               SMLoc &EndLoc) override;

  bool parseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;

  bool matchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

public:
  RCA1802AsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,
               const MCInstrInfo &MII, const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII) {
    setAvailableFeatures(ComputeAvailableFeatures(STI.getFeatureBits()));
  }
};

} // end anonymous namespace

bool RCA1802AsmParser::parseRegister(MCRegister &Reg, SMLoc &StartLoc,
                                 SMLoc &EndLoc) {
  return tryParseRegister(Reg, StartLoc, EndLoc).isSuccess();
}

ParseStatus RCA1802AsmParser::tryParseRegister(MCRegister &Reg, SMLoc &StartLoc,
                                           SMLoc &EndLoc) {
  const AsmToken &Tok = getParser().getTok();
  StartLoc = Tok.getLoc();
  EndLoc = Tok.getEndLoc();

  if (Tok.isNot(AsmToken::Identifier))
    return ParseStatus::NoMatch;

  std::string Name = Tok.getString().lower();
  MCRegister R = MatchRegisterName(Name);
  
  if (R == RCA1802::NoRegister)
    return ParseStatus::NoMatch;

  Reg = R;
  getParser().Lex();
  return ParseStatus::Success;
}

bool RCA1802AsmParser::parseInstruction(ParseInstructionInfo &Info,
                                    StringRef Name, SMLoc NameLoc,
                                    OperandVector &Operands) {
  Operands.push_back(RCA1802Operand::createToken(Name, NameLoc));

  while (getLexer().isNot(AsmToken::EndOfStatement)) {
    SMLoc S = getLexer().getLoc();
    MCRegister Reg;
    SMLoc SReg, EReg;
    if (tryParseRegister(Reg, SReg, EReg).isSuccess()) {
      Operands.push_back(RCA1802Operand::createReg(Reg, SReg, EReg));
    } else {
      const MCExpr *Imm;
      if (!getParser().parseExpression(Imm)) {
        SMLoc E = getLexer().getLoc();
        Operands.push_back(RCA1802Operand::createImm(Imm, S, E));
      } else {
        return true;
      }
    }

    if (getLexer().is(AsmToken::Comma))
      getLexer().Lex();
  }

  return false;
}

bool RCA1802AsmParser::matchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                                           OperandVector &Operands,
                                           MCStreamer &Out, uint64_t &ErrorInfo,
                                           bool MatchingInlineAsm) {
  MCInst Inst;
  unsigned MatchResult =
      MatchInstructionImpl(Operands, Inst, nullptr, MatchingInlineAsm);

  switch (MatchResult) {
  case Match_Success:
    Inst.setLoc(IDLoc);
    Out.emitInstruction(Inst, getSTI());
    return false;
  case Match_MnemonicFail:
    return Error(IDLoc, "invalid instruction mnemonic");
  case Match_InvalidOperand: {
    SMLoc ErrorLoc = IDLoc;
    if (ErrorInfo != ~0ULL) {
      if (ErrorInfo >= Operands.size())
        return Error(IDLoc, "too few operands for instruction");

      ErrorLoc = ((RCA1802Operand &)*Operands[ErrorInfo]).getStartLoc();
      if (ErrorLoc == SMLoc())
        ErrorLoc = IDLoc;
    }
    return Error(ErrorLoc, "invalid operand for instruction");
  }
  default:
    return Error(IDLoc, "unknown instruction match error");
  }
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRCA1802AsmParser() {
  RegisterMCAsmParser<RCA1802AsmParser> X(getTheRCA1802Target());
}

#define GET_MATCHER_IMPLEMENTATION
#include "RCA1802GenAsmMatcher.inc"
