//===-- RCA1802MCTargetDesc.cpp - RCA1802 Target Descriptions ---------------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RCA1802MCTargetDesc.h"
#include "MCTargetDesc/RCA1802AsmBackend.h"
#include "MCTargetDesc/RCA1802InstPrinter.h"
#include "MCTargetDesc/RCA1802MCAsmInfo.h"
#include "MCTargetDesc/RCA1802MCELFStreamer.h"
#include "TargetInfo/RCA1802TargetInfo.h"

#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/TargetParser/Triple.h"

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "RCA1802GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "RCA1802GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "RCA1802GenRegisterInfo.inc"

namespace llvm {

MCInstrInfo *createRCA1802MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitRCA1802MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createRCA1802MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitRCA1802MCRegisterInfo(X, RCA1802::R3); // RPC is Program Counter
  return X;
}

static MCSubtargetInfo *createRCA1802MCSubtargetInfo(const Triple &TT,
                                                StringRef CPU, StringRef FS) {
  return createRCA1802MCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCAsmInfo *createRCA1802MCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT,
                                     const MCTargetOptions &Options) {
  return new RCA1802MCAsmInfo(TT, Options);
}

static MCInstPrinter *createRCA1802MCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  return new RCA1802InstPrinter(MAI, MII, MRI);
}

namespace RCA1802 {
bool isZeroPageSectionName(StringRef Name) {
  return Name == ".zp" || Name.starts_with(".zp.") || Name == ".direct" ||
         Name.starts_with(".direct.");
}
} // namespace RCA1802

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRCA1802TargetMC() {
  for (Target *T : {&getTheRCA1802Target()}) {
    RegisterMCAsmInfoFn X(*T, createRCA1802MCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(*T, createRCA1802MCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(*T, createRCA1802MCRegisterInfo);
    TargetRegistry::RegisterMCSubtargetInfo(*T, createRCA1802MCSubtargetInfo);
    TargetRegistry::RegisterMCInstPrinter(*T, createRCA1802MCInstPrinter);
    TargetRegistry::RegisterMCAsmBackend(*T, createRCA1802AsmBackend);
    TargetRegistry::RegisterELFStreamer(*T, createRCA1802MCELFStreamer);
    TargetRegistry::RegisterMCCodeEmitter(*T, createRCA1802MCCodeEmitter);
  }
}

} // namespace llvm
