add_llvm_component_library(LLVMRCA1802AsmParser
  RCA1802AsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  RCA1802Desc
  RCA1802Info
  Support

  ADD_TO_COMPONENT
  RCA1802
)
