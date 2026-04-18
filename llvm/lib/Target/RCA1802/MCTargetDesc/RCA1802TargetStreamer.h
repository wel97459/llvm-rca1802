//===-- RCA1802TargetStreamer.h - RCA1802 Target Streamer --------------*- C++ -*--===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RCA1802_TARGET_STREAMER_H
#define LLVM_RCA1802_TARGET_STREAMER_H

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/FormattedStream.h"

namespace llvm {
class MCStreamer;

/// A generic RCA1802 target output stream.
class RCA1802TargetStreamer : public MCTargetStreamer {
public:
  explicit RCA1802TargetStreamer(MCStreamer &S);

  void finish() override;
  virtual bool emitDirectiveZeroPage(MCSymbol *Symbol) = 0;

protected:
  virtual bool hasBSS() = 0;
  virtual bool hasZPBSS() = 0;
  virtual bool hasData() = 0;
  virtual bool hasZPData() = 0;
  virtual bool hasInitArray() = 0;
  virtual bool hasFiniArray() = 0;

  void stronglyReference(StringRef Name, StringRef Comment);
  virtual void stronglyReference(MCSymbol *Sym) = 0;
};

/// A target streamer for textual RCA1802 assembly code.
class RCA1802TargetAsmStreamer final : public RCA1802TargetStreamer {
public:
  RCA1802TargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);

private:
  formatted_raw_ostream &OS;

  bool emitDirectiveZeroPage(MCSymbol *Symbol) override {
    OS << "\t.zeropage\t" << Symbol->getName() << "\n";
    return true;
  };

  void changeSection(const MCSection *CurSection, MCSection *Section,
                     uint32_t SubSection, raw_ostream &OS) override;

  bool hasBSS() override { return HasBSS; }
  bool hasZPBSS() override { return HasZPBSS; }
  bool hasData() override { return HasData; }
  bool hasZPData() override { return HasZPData; }
  bool hasInitArray() override { return HasInitArray; }
  bool hasFiniArray() override { return HasFiniArray; }

  void stronglyReference(MCSymbol *Sym) override;

  bool HasBSS = false;
  bool HasZPBSS = false;
  bool HasData = false;
  bool HasZPData = false;
  bool HasInitArray = false;
  bool HasFiniArray = false;
};

inline RCA1802TargetAsmStreamer::RCA1802TargetAsmStreamer(MCStreamer &S,
                                                  formatted_raw_ostream &OS)
    : RCA1802TargetStreamer(S), OS(OS) {}

/// A target streamer for an RCA1802 ELF object file.
class RCA1802TargetELFStreamer final : public RCA1802TargetStreamer {
public:
  RCA1802TargetELFStreamer(MCStreamer &S, const MCSubtargetInfo &STI);

private:
  MCELFStreamer &getStreamer() {
    return static_cast<MCELFStreamer &>(Streamer);
  }

  bool emitDirectiveZeroPage(MCSymbol *Symbol) override;
  bool hasBSS() override;
  bool hasZPBSS() override;
  bool hasData() override;
  bool hasZPData() override;
  bool hasInitArray() override;
  bool hasFiniArray() override;

  void stronglyReference(MCSymbol *Sym) override;
};

} // end namespace llvm

#endif // LLVM_RCA1802_TARGET_STREAMER_H
