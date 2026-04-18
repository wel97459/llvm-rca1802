//===- RCA1802TargetTransformInfo.h - RCA1802 specific TTI --------------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines a TargetTransformInfo::Concept conforming object specific
// to the RCA1802 target machine. It uses the target's detailed information to
// provide more precise answers to certain TTI queries, while letting the
// target-independent and default TTI implementations handle the rest.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RCA1802_RCA1802TARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_RCA1802_RCA1802TARGETTRANSFORMINFO_H

#include "RCA1802TargetMachine.h"
#include "llvm/CodeGen/BasicTTIImpl.h"
#include "llvm/Support/BranchProbability.h"

namespace llvm {

class RCA1802TTIImpl : public BasicTTIImplBase<RCA1802TTIImpl> {
  using BaseT = BasicTTIImplBase<RCA1802TTIImpl>;

  friend BaseT;

  const RCA1802Subtarget *ST;
  const RCA1802TargetLowering *TLI;

  const RCA1802Subtarget *getST() const { return ST; }
  const RCA1802TargetLowering *getTLI() const { return TLI; }

public:
  explicit RCA1802TTIImpl(const RCA1802TargetMachine *TM, const Function &F)
      : BaseT(TM, F.getParent()->getDataLayout()), ST(TM->getSubtargetImpl(F)),
        TLI(ST->getTargetLowering()) {}

  // All div, rem, and divrem ops are libcalls, so any possible combination
  // exists.
  bool hasDivRemOp(Type *DataType, bool IsSigned) const override {
    return true;
  }

  bool allowIllegalIntegerIV() const override { return true; }

  bool isLSRCostLess(const TargetTransformInfo::LSRCost &C1,
                     const TargetTransformInfo::LSRCost &C2) const override {
    // Prefer instruction count to the other metrics.
    return std::tie(C1.Insns, C1.NumRegs, C1.AddRecCost, C1.NumIVMuls,
                    C1.NumBaseAdds, C1.ScaleCost, C1.ImmCost, C1.SetupCost) <
           std::tie(C2.Insns, C2.NumRegs, C2.AddRecCost, C2.NumIVMuls,
                    C2.NumBaseAdds, C2.ScaleCost, C2.ImmCost, C2.SetupCost);
  }

  BranchProbability getPredictableBranchThreshold() const override {
    return BranchProbability(0, 1);
  }

  bool isValidAddrSpaceCast(unsigned FromAS, unsigned ToAS) const override {
    return true;
  }

  bool strictInliningCosts() const override { return true; }
};

} // end namespace llvm

#endif // not LLVM_LIB_TARGET_RCA1802_RCA1802TARGETTRANSFORMINFO_H
