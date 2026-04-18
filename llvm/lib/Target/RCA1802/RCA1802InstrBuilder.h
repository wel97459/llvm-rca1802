//===-- RCA1802InstrBuilder.h - RCA1802 Instruction Builder ------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INSTRBUILDER_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INSTRBUILDER_H

#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "RCA1802RegisterInfo.h"
#include "RCA1802Subtarget.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"

namespace llvm {

inline unsigned getIncPseudoOpcode(const MachineIRBuilder &Builder) {
  return RCA1802::INC;
}

inline unsigned getDecPseudoOpcode(const MachineIRBuilder &Builder) {
  return RCA1802::DEC;
}

template <typename DstOp>
inline MachineInstrBuilder buildLdImm(MachineIRBuilder &Builder, DstOp Dest) {
  return Builder.buildInstr(RCA1802::LDI, {Dest}, {});
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_RCA1802_RCA1802INSTRBUILDER_H
