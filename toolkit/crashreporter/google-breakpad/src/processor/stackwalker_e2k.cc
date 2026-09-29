// Copyright (c) 2013 Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "common/scoped_ptr.h"
#include "google_breakpad/processor/call_stack.h"
#include "google_breakpad/processor/code_modules.h"
#include "google_breakpad/processor/memory_region.h"
#include "google_breakpad/processor/source_line_resolver_interface.h"
#include "google_breakpad/processor/stack_frame_cpu.h"
#include "processor/cfi_frame_info.h"
#include "processor/logging.h"
#include "processor/postfix_evaluator-inl.h"
#include "processor/stackwalker_e2k.h"
#include "processor/windows_frame_info.h"
#include "google_breakpad/common/minidump_cpu_e2k.h"

namespace google_breakpad {

StackwalkerE2K::StackwalkerE2K(const SystemInfo* system_info,
                               const MDRawContextE2K* context,
                               MemoryRegion* memory,
                               MemoryRegion* chain_stack,
                               MemoryRegion* procedure_stack,
                               const CodeModules* modules,
                               StackFrameSymbolizer* resolver_helper)
: Stackwalker(system_info, memory, modules, resolver_helper),
  context_(context), chain_stack_(chain_stack),
  procedure_stack_(procedure_stack) {
}

StackFrame* StackwalkerE2K::GetContextFrame() {
  if (!context_) {
    BPLOG(ERROR) << "Can't get context frame without context.";
    return NULL;
  }

  StackFrameE2K* frame = new StackFrameE2K();
  frame->context = *context_;
  frame->context_validity = StackFrameE2K::CONTEXT_VALID_ALL;
  frame->trust = StackFrame::FRAME_TRUST_CF;
  frame->instruction = frame->context.cr0_hi & 0xfffffffffff8;
  // skip first frame(failed) its already in cr-s.
  frame->pcs = (frame->context.pcsp_lo & 0xffffffffffff) +
               (frame->context.pcsp_hi & 0xffffffff) - 0x20;
  frame->sp = frame->context.usd_lo & 0xffffffffffff; // [rwap base [47: 0]
  frame->stack_size = (frame->context.usd_hi >> 32) & 0xffffffff; // [rwap size 63:32]
  return frame;
}

StackFrameE2K* StackwalkerE2K::GetCallerByStacks(
    const vector<StackFrame*>& frames) {
  StackFrameE2K* last_frame = static_cast<StackFrameE2K*>(frames.back());
  StackFrameE2K* frame = new StackFrameE2K();
  // Get caller ip from chain stack.
  // Read previous cr(0x20), and get ip and stack size from it
  uint64_t cr0_hi, cr1_hi = 0;
  uint64_t previous_cr = last_frame->pcs - 0x20;
  if(!chain_stack_->GetMemoryAtAddress(previous_cr + 0x8, &cr0_hi) ||
     !chain_stack_->GetMemoryAtAddress(previous_cr + 0x18, &cr1_hi)) {
    BPLOG(INFO) << " GetMemoryAtAddress for ip failed" ;
    return NULL;
  }

  if (!cr0_hi) return NULL; // End of backtrace
  frame->context = last_frame->context;
  frame->context_validity = StackFrameE2K::CONTEXT_VALID_ALL;
  frame->trust = StackFrame::FRAME_TRUST_CF;
  frame->instruction = (cr0_hi & 0xfffffffffff8); // [VA_MSB:0] 8-aligned
  frame->pcs = previous_cr;
  // usd_base + (ussz - usd_size)
  frame->sp = last_frame->sp + (((cr1_hi >> 32) & 0xfffffff0) -
      last_frame->stack_size);
  frame->stack_size = (cr1_hi >> 32) & 0xfffffff0; // [63:36] 16-aligned
  return frame;
}

StackFrame* StackwalkerE2K::GetCallerFrame(const CallStack* stack,
                                            bool stack_scan_allowed) {
  if (!context_ || !stack) {
    BPLOG(ERROR) << "Can't get caller frame without callee context";
    return NULL;
  }

  const vector<StackFrame*>& frames = *stack->frames();
  scoped_ptr<StackFrameE2K> new_frame;

  if (stack_scan_allowed && !new_frame.get()) {
    new_frame.reset(GetCallerByStacks(frames));
  }

  // If nothing worked, tell the caller.
  if (!new_frame.get()) {
    return NULL;
  }

  return new_frame.release();
}

}  // namespace google_breakpad
