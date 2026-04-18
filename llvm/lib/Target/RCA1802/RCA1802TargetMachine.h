//===-- RCA1802TargetMachine.h - Define TargetMachine for RCA1802 -------*- C++ -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the RCA1802 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RCA1802_TARGET_MACHINE_H
#define LLVM_RCA1802_TARGET_MACHINE_H

#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

#include "RCA1802FrameLowering.h"
#include "RCA1802ISelLowering.h"
#include "RCA1802InstrInfo.h"
#include "RCA1802Subtarget.h"

namespace llvm {

/// A generic RCA1802 implementation.
class RCA1802TargetMachine : public CodeGenTargetMachineImpl {
public:
  RCA1802TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                   bool JIT);
  ~RCA1802TargetMachine() override;

  const RCA1802Subtarget *getSubtargetImpl() const { return &SubTarget; }
  const RCA1802Subtarget *getSubtargetImpl(const Function &F) const override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return this->TLOF.get();
  }

  TargetTransformInfo getTargetTransformInfo(const Function &F) const override;

  bool hasNoInitSection() const override { return true; }

  void registerPassBuilderCallbacks(PassBuilder &) override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  // The 6502 has only register-related scheduling concerns, so disable PostRA
  // scheduling by claiming to emit it ourselves, then never doing so.
  bool targetSchedulesPostRAScheduling() const override { return true; };

  StringRef getSectionPrefix(const GlobalObject *GO) const override;

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;

  ScheduleDAGInstrs *
  createMachineScheduler(MachineSchedContext *C) const override;

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  RCA1802Subtarget SubTarget;
  mutable StringMap<std::unique_ptr<RCA1802Subtarget>> SubtargetMap;
};

} // end namespace llvm

#endif // LLVM_RCA1802_TARGET_MACHINE_H
