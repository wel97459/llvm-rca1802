//===-- RCA1802.h - Top-level interface for RCA1802 representation ------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// RCA1802 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802_H

#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/Pass.h"

namespace llvm {

void initializeRCA1802CombinerPass(PassRegistry &);
void initializeRCA1802CopyOptPass(PassRegistry &);
void initializeRCA1802IndexIVPass(PassRegistry &);
void initializeRCA1802InsertCopiesPass(PassRegistry &);
void initializeRCA1802InternalizePass(PassRegistry &);
void initializeRCA1802LateOptimizationPass(PassRegistry &);
void initializeRCA1802LowerSelectPass(PassRegistry &);
void initializeRCA1802NonReentrantPass(PassRegistry &);
void initializeRCA1802PostRAScavengingPass(PassRegistry &);
void initializeRCA1802ShiftRotateChainPass(PassRegistry &);
void initializeRCA1802StaticStackAllocPass(PassRegistry &);
void initializeRCA1802ZeroPageAllocPass(PassRegistry &);

// The behind-by-one property of the std::reverse_iterator adaptor applied by
// reverse() does not properly handle instruction erasures. This range construct
// converts the forward iterators to native reverse iterators that are not
// behind-by-one and therefore handle erasures correctly when combined with
// make_early_inc_range().
inline auto mbb_reverse(MachineBasicBlock::iterator Begin,
                        MachineBasicBlock::iterator End) {
  return make_range(MachineBasicBlock::reverse_iterator(End),
                    MachineBasicBlock::reverse_iterator(Begin));
}
template <typename ContainerTy> inline auto mbb_reverse(ContainerTy &&C) {
  return mbb_reverse(C.begin(), C.end());
}

} // namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802_H
