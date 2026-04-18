add_llvm_component_library(LLVMRCA1802Desc
  RCA1802AsmBackend.cpp
  RCA1802ELFObjectWriter.cpp
  RCA1802FixupKinds.cpp
  RCA1802InstPrinter.cpp
  RCA1802MCAsmInfo.cpp
  RCA1802MCCodeEmitter.cpp
  RCA1802MCELFStreamer.cpp
  RCA1802MCExpr.cpp
  RCA1802MCInstrAnalysis.cpp
  RCA1802MCTargetDesc.cpp
  RCA1802ModifierNames.cpp
  RCA1802TargetStreamer.cpp

  LINK_COMPONENTS
  MC
  RCA1802Info
  Support

  ADD_TO_COMPONENT
  RCA1802
)

add_dependencies(LLVMRCA1802Desc RCA1802CommonTableGen)
