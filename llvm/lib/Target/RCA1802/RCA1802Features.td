//===-- RCA1802Features.td - RCA1802 Features ---------------------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

def FeatureRCA1802 : SubtargetFeature<"rca1802", "HasRCA1802", "true",
                                     "RCA 1802 instruction set">;

def FeatureRCA1804 : SubtargetFeature<"rca1804", "HasRCA1804", "true",
                        "RCA 1804 instruction set (includes 1802)",
                        [FeatureRCA1802]>;

def FeatureStaticStack :
  SubtargetFeature<
    "static-stack", "StaticStack", "true",
    "Whether to use statically-allocated stack frames if possible.">;

// MOS-compatible dummy features (expected by copied C++ code)
def FeatureW65816 : SubtargetFeature<"w65816", "HasW65816", "true", "dummy">;
def Feature65CE02 : SubtargetFeature<"65ce02", "Has65CE02", "true", "dummy">;
def FeatureHUC6280 : SubtargetFeature<"huc6280", "HasHUC6280", "true", "dummy">;
def FeatureSPC700 : SubtargetFeature<"spc700", "HasSPC700", "true", "dummy">;
def FeatureJMPIdxIndir : SubtargetFeature<"jmp-idx-indir", "HasJMPIdxIndir", "true", "dummy">;
def Feature45GS02 : SubtargetFeature<"45gs02", "Has45GS02", "true", "dummy">;
def Feature65EL02 : SubtargetFeature<"65el02", "Has65EL02", "true", "dummy">;
def FeatureGPRIncDec : SubtargetFeature<"gpr-inc-dec", "HasGPRIncDec", "true", "dummy">;
