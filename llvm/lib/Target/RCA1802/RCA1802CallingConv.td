//===-- RCA1802CallingConv.td - Calling Conventions RCA1802 --------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the RCA1802 architecture.
//
//===----------------------------------------------------------------------===//

def CC_RCA1802_Impl : CallingConv<[
  CCIfType<[i1], CCPromoteToType<i8>>,
  
  // 16-bit values (and pointers) are assigned to R7-R15.
  CCIfType<[i16], CCAssignToReg<[R7, R8, R9, R10, R11, R12, R13, R14, R15]>>,

  // 8-bit values are assigned to D (Ac) if possible.
  CCIfType<[i8], CCAssignToReg<[D]>>,

  // Values that wouldn't fit anywhere else are passed directly on the stack.
  CCAssignToStack<0, 2>,
]>;

def CC_RCA1802_VarArgs_Impl : CallingConv<[
  CCIfType<[i1], CCPromoteToType<i8>>,
  CCAssignToStack<0, 2>,
]>;

// Callee-saved registers. R7-R15 are used for arguments, let's say R10-R15 are saved if not used.
// This is just a placeholder to satisfy the build.
def RCA1802_CSR : CalleeSavedRegs<(add R10, R11, R12, R13, R14, R15)>;

def RCA1802_Interrupt_CSR : CalleeSavedRegs<(add ALL16, D, DF, Q, P, X)>;
