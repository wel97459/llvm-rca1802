//===-- RCA1802Devices.td - RCA1802 Devices -----------------------*- tablegen -*-===//
//
// Part of LLVM-RCA1802, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//
// RCA1802 Device Definitions
//===---------------------------------------------------------------------===//

def RCA1802SchedModel : SchedMachineModel {
  let MicroOpBufferSize = 1;
  let CompleteModel = false;
}

class RCA1802Processor<string Name, list<SubtargetFeature> Features>
  : ProcessorModel<Name, RCA1802SchedModel, Features>;

def : RCA1802Processor<"rca1802", [FeatureRCA1802]>;
def : RCA1802Processor<"rca1804", [FeatureRCA1804]>;
def : RCA1802Processor<"rca1805", [FeatureRCA1804]>;
def : RCA1802Processor<"rca1806", [FeatureRCA1804]>;
