; RUN: llc -mtriple=rca1802-unknown-unknown -version | FileCheck %s
; CHECK: rca1802 - RCA1802

target triple = "rca1802-unknown-unknown"

define void @foo() {
  ret void
}
