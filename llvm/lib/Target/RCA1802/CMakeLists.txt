add_llvm_component_group(RCA1802)

set(LLVM_TARGET_DEFINITIONS RCA1802.td)

tablegen(LLVM RCA1802GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM RCA1802GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM RCA1802GenCallingConv.inc -gen-callingconv)
tablegen(LLVM RCA1802GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM RCA1802GenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM RCA1802GenGICombiner.inc -gen-global-isel-combiner -combiners="RCA1802Combiner")
tablegen(LLVM RCA1802GenGlobalISel.inc -gen-global-isel)
tablegen(LLVM RCA1802GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RCA1802GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM RCA1802GenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM RCA1802GenRegisterBank.inc -gen-register-bank)
tablegen(LLVM RCA1802GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RCA1802GenSearchableTables.inc -gen-searchable-tables)
tablegen(LLVM RCA1802GenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(RCA1802CommonTableGen)

add_llvm_target(RCA1802CodeGen
  RCA1802AsmPrinter.cpp
  RCA1802CallGraphUtils.cpp
  RCA1802CallLowering.cpp
  RCA1802CallingConv.cpp
  RCA1802Combiner.cpp
  RCA1802CopyOpt.cpp
  RCA1802FrameLowering.cpp
  RCA1802ISelLowering.cpp
  RCA1802IndexIV.cpp
  RCA1802InlineAsmLowering.cpp
  RCA1802InsertCopies.cpp
  RCA1802InstrCost.cpp
  RCA1802InstrInfo.cpp
  RCA1802InstructionSelector.cpp
  RCA1802Internalize.cpp
  RCA1802LateOptimization.cpp
  RCA1802LegalizerInfo.cpp
  RCA1802LowerSelect.cpp
  RCA1802MCInstLower.cpp
  RCA1802MachineScheduler.cpp
  RCA1802NonReentrant.cpp
  RCA1802PostRAScavenging.cpp
  RCA1802RegisterBankInfo.cpp
  RCA1802RegisterInfo.cpp
  RCA1802ShiftRotateChain.cpp
  RCA1802StaticStackAlloc.cpp
  RCA1802Subtarget.cpp
  RCA1802TargetMachine.cpp
  RCA1802TargetObjectFile.cpp
  RCA1802ZeroPageAlloc.cpp

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  BinaryFormat
  CodeGen
  CodeGenTypes
  Core
  GlobalISel
  IPO
  InstCombine
  LTO
  MC
  RCA1802Desc
  RCA1802Info
  Passes
  Scalar
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  RCA1802
)

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
