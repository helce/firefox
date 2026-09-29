/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Platform specific code to invoke XPCOM methods on native objects */

#include "xptcprivate.h"

#ifdef __clang__
#define MOZ_NO_TAILCALL __attribute__((disable_tail_calls))
#else
#define MOZ_NO_TAILCALL
#endif

/* e2k ABI passes the first 8 parameters in registers. Both floating point
 * and integer accepted. The rest of the parameters are passed in the caller
 * stack area. In this case, first 8 parameter slots should be reserved.
 */

const uint32_t GPR_COUNT = 8;

static void InvokeCopyToStack(uint32_t paramCount, nsXPTCVariant *s,
                              uint64_t *stack, uint64_t *gpregs)
{
    uint32_t nr_gpr = 0u;
    uint64_t value = 0u;
    for (uint32_t i = 0; i < paramCount; i++, s++) {
        if (s->IsIndirect()) {
            value = (uint64_t) &s->val;
        } else {
            switch (s->type) {
            case nsXPTType::T_I8:     value = s->val.i8;                break;
            case nsXPTType::T_I16:    value = s->val.i16;               break;
            case nsXPTType::T_I32:    value = s->val.i32;               break;
            case nsXPTType::T_I64:    value = s->val.i64;               break;
            case nsXPTType::T_U8:     value = s->val.u8;                break;
            case nsXPTType::T_U16:    value = s->val.u16;               break;
            case nsXPTType::T_U32:    value = s->val.u32;               break;
            case nsXPTType::T_U64:    value = s->val.u64;               break;
            case nsXPTType::T_BOOL:   value = s->val.b;                 break;
            case nsXPTType::T_CHAR:   value = s->val.c;                 break;
            case nsXPTType::T_WCHAR:  value = s->val.wc;                break;
            case nsXPTType::T_FLOAT:  value = s->val.u32;               break;
            case nsXPTType::T_DOUBLE: value = s->val.u64;               break;
            default:                  value = (uint64_t) s->val.p;      break;
            }
        }
        if (nr_gpr < GPR_COUNT - 1) {
            gpregs[nr_gpr++] = value;
        } else {
            *stack++ = value;
        }
    }
}

typedef nsresult (*vtable_func)(nsISupports *, uint64_t, uint64_t, uint64_t,
                                uint64_t, uint64_t, uint64_t, uint64_t);

MOZ_ASAN_IGNORE
MOZ_NO_TAILCALL
EXPORT_XPCOM_API(nsresult)
NS_InvokeByIndex(nsISupports *that, uint32_t methodIndex, uint32_t paramCount,
                 nsXPTCVariant *params)
{
    vtable_func *vtable = *reinterpret_cast<vtable_func **>(that);
    vtable_func method = vtable[methodIndex];

    uint64_t gpregs[GPR_COUNT - 1] = { 0 };
    uint32_t stack_slots = 0, reg_slots = 1; // Skip first slot for 'that'
    uint64_t *stack = 0;

    for (uint32_t i = 0; i < paramCount; i++) 
        if (reg_slots < GPR_COUNT) reg_slots++; else stack_slots++;

    if (stack_slots) {
        stack_slots = (stack_slots + 1) & ~1; // Stack, if used, must be 16-bytes aligned
        // Reserve space for GPR_COUNT slots.
        stack = reinterpret_cast<uint64_t *>(__builtin_alloca((stack_slots + GPR_COUNT) * 8));
    }

    InvokeCopyToStack(paramCount, params, stack, gpregs);
    return method(that, gpregs[0], gpregs[1], gpregs[2], gpregs[3], gpregs[4],
                  gpregs[5], gpregs[6]);
}
