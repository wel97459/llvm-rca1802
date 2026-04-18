//===-- RCA1802TargetObjectFile.h - RCA1802 Object Info -----------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802TARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {

class RCA1802TargetObjectFile : public TargetLoweringObjectFileELF {
public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

  MCSection *SelectSectionForGlobal(const GlobalObject *GO, SectionKind Kind,
                                    const TargetMachine &TM) const override;

  MCSection *getExplicitSectionGlobal(const GlobalObject *GO, SectionKind Kind,
                                      const TargetMachine &TM) const override {
    return TargetLoweringObjectFileELF::getExplicitSectionGlobal(GO, Kind, TM);
  }
};

} // end namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802TARGETOBJECTFILE_H
