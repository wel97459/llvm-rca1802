add_llvm_component_library(LLVMRCA1802Disassembler
  RCA1802Disassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  RCA1802Desc
  RCA1802Info
  Support

  ADD_TO_COMPONENT
  RCA1802
)


