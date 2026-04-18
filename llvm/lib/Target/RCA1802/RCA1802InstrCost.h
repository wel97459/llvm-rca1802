//===-- RCA1802InstrCost.h - RCA1802 Instruction Cost structure ---------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the definition of the RCA1802InstrCost class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802INSTRCOST_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802INSTRCOST_H

#include "llvm/CodeGen/MachineFunction.h"
#include <cstdint>

namespace llvm {

class RCA1802InstrCost {
public:
  enum class Mode {
    PreferBytes,
    PreferCycles,
    Average
  };

  RCA1802InstrCost() : Bytes(0), Cycles(0) {}

  RCA1802InstrCost(int32_t Bytes, int32_t Cycles)
    : RCA1802InstrCost(Bytes, Cycles, 256) {}

  friend RCA1802InstrCost operator+(RCA1802InstrCost Left,
                                const RCA1802InstrCost& Right) {
    return RCA1802InstrCost(Left.Bytes + Right.Bytes,
                        Left.Cycles + Right.Cycles, 1);
  }

  RCA1802InstrCost& operator+=(const RCA1802InstrCost& Right) {
    this->Bytes += Right.Bytes;
    this->Cycles += Right.Cycles;
    return *this;
  }

  friend RCA1802InstrCost operator-(RCA1802InstrCost Left,
                                const RCA1802InstrCost& Right) {
    return RCA1802InstrCost(Left.Bytes - Right.Bytes,
                        Left.Cycles - Right.Cycles, 1);
  }

  RCA1802InstrCost& operator-=(const RCA1802InstrCost& Right) {
    this->Bytes -= Right.Bytes;
    this->Cycles -= Right.Cycles;
    return *this;
  }

  friend RCA1802InstrCost operator*(RCA1802InstrCost Left, int Right) {
    return RCA1802InstrCost(Left.Bytes * Right, Left.Cycles * Right, 1);
  }

  friend RCA1802InstrCost operator/(RCA1802InstrCost Left, int Right) {
    return RCA1802InstrCost(Left.Bytes / Right, Left.Cycles / Right, 1);
  }

  int64_t value(Mode Mode = Mode::Average) const;

  static Mode getModeFor(const MachineFunction &MF);

private:
  RCA1802InstrCost(int32_t Bytes, int32_t Cycles, int Multiplier)
    : Bytes(Bytes * Multiplier), Cycles(Cycles * Multiplier) {}

  int32_t Bytes, Cycles;
};

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802CYCLECOST_H
