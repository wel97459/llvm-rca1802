//===-- RCA1802MCAsmInfo.h - RCA1802 asm properties ---------------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the RCA1802MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RCA1802_ASM_INFO_H
#define LLVM_RCA1802_ASM_INFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

/// Specifies the format of RCA1802 assembly files.
class RCA1802MCAsmInfo : public MCAsmInfoELF {
public:
  explicit RCA1802MCAsmInfo(const Triple &TT, const MCTargetOptions &Options);

  unsigned getMaxInstLength(const MCSubtargetInfo *STI) const override;
};

} // end namespace llvm

#endif // LLVM_RCA1802_ASM_INFO_H
