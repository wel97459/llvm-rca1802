//===-- RCA1802TargetMachine.cpp - Define TargetMachine for RCA1802 ---------------===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the RCA1802 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#include "RCA1802TargetMachine.h"
#include "RCA1802.h"
#include "RCA1802MachineFunctionInfo.h"
#include "RCA1802Subtarget.h"
#include "RCA1802TargetObjectFile.h"
#include "RCA1802TargetTransformInfo.h"
#include "MCTargetDesc/RCA1802MCTargetDesc.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/Scalar.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRCA1802Target() {
  RegisterTargetMachine<RCA1802TargetMachine> X(getTheRCA1802Target());
}

static std::string computeDataLayout(const Triple &TT) {
  // RCA1802 is big-endian
  // Pointers are 16-bit
  return "E-m:e-p:16:8-i8:8-i16:8-i32:8-i64:8-f32:8-f64:8-a:8-n8:16";
}

static Reloc::Model getRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

RCA1802TargetMachine::RCA1802TargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, computeDataLayout(TT), TT, CPU, FS, Options,
                        getRelocModel(RM),
                        CM.value_or(CodeModel::Small), OL),
      TLOF(std::make_unique<RCA1802TargetObjectFile>()),
      SubTarget(TT, CPU.str(), FS.str(), *this) {
  initAsmInfo();
}

RCA1802TargetMachine::~RCA1802TargetMachine() = default;

const RCA1802Subtarget *
RCA1802TargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = CPUAttr.isValid() ? CPUAttr.getValueAsString().str() : TargetCPU;
  std::string FS = FSAttr.isValid() ? FSAttr.getValueAsString().str() : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will use DataLayout which is changed by target out of the
    // constructor.
    resetTargetOptions(F);
    I = std::make_unique<RCA1802Subtarget>(TargetTriple, CPU, FS, *this);
  }
  return I.get();
}

TargetTransformInfo
RCA1802TargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(std::make_unique<RCA1802TTIImpl>(this, F));
}

void RCA1802TargetMachine::registerPassBuilderCallbacks(PassBuilder &PB) {}

MachineFunctionInfo *RCA1802TargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return new (Allocator) RCA1802FunctionInfo(F, static_cast<const RCA1802Subtarget *>(STI));
}

ScheduleDAGInstrs *
RCA1802TargetMachine::createMachineScheduler(MachineSchedContext *C) const {
  return nullptr;
}

namespace {
class RCA1802PassConfig : public TargetPassConfig {
public:
  RCA1802PassConfig(RCA1802TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  RCA1802TargetMachine &getRCA1802TargetMachine() const {
    return getTM<RCA1802TargetMachine>();
  }

  bool addInstSelector() override;
  void addIRPasses() override;
};
} // namespace

TargetPassConfig *RCA1802TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new RCA1802PassConfig(*this, PM);
}

bool RCA1802PassConfig::addInstSelector() {
  addPass(new Legalizer());
  addPass(new RegBankSelect());
  addPass(new InstructionSelect());
  return false;
}

void RCA1802PassConfig::addIRPasses() {
  TargetPassConfig::addIRPasses();
}

StringRef RCA1802TargetMachine::getSectionPrefix(const GlobalObject *GO) const {
  if (GO->getAddressSpace() == RCA1802::AS_ZeroPage)
    return ".zp";
  return "";
}
