/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Implement shared vtbl methods

#include "xptcprivate.h"

const uint32_t GPR_COUNT            = 8;

/*
 * PrepareAndDispatch() is called by SharedStub() and calls the actual method.
 *
 * - 'args[]' contains the arguments passed on stack
 * - 'gpregs[]' contains the arguments passed in registers
 *
 * The parameters are mapped into an array of type 'nsXPTCMiniVariant' and then
 * the method gets called
 */ 

extern "C" nsresult ATTRIBUTE_USED
PrepareAndDispatch(nsXPTCStubBase * self, uint32_t methodIndex,
                   uint64_t * args, uint64_t * gpregs)
{
    nsXPTCMiniVariant paramBuffer[PARAM_BUFFER_COUNT];
    const nsXPTMethodInfo* info;
    uint32_t paramCount;

    NS_ASSERTION(self,"no self");

    self->mEntry->GetMethodInfo(uint16_t(methodIndex), &info);
    NS_ASSERTION(info,"no method info");
    if (!info)
        return NS_ERROR_UNEXPECTED;

    paramCount = info->ParamCount();

    const uint8_t indexOfJSContext = info->IndexOfJSContext();

    uint64_t* ap = args;
    uint32_t nr_gpr = 1;    // skip one GPR register for 'that'
    uint64_t value;

    for (uint32_t i = 0; i < paramCount; i++) {
        const nsXPTParamInfo& param = info->Param(i);
        const nsXPTType& type = param.GetType();
        nsXPTCMiniVariant* dp = &paramBuffer[i];

        if (i == indexOfJSContext) {
            if (nr_gpr < GPR_COUNT)
                nr_gpr++;
            else
                ap++;
        }

        if (!param.IsOut() && type == nsXPTType::T_DOUBLE) {
            if (nr_gpr < GPR_COUNT)
                dp->val.d = *(double*) &gpregs[nr_gpr++];
            else
                dp->val.d = *(double*) ap++;
            continue;
        }

        if (!param.IsOut() && type == nsXPTType::T_FLOAT) {
            if (nr_gpr < GPR_COUNT)
                dp->val.f = *(float*) &gpregs[nr_gpr++];
            else
                dp->val.f = *(float*) ap++;
            continue;
        }

        if (nr_gpr < GPR_COUNT)
            value = gpregs[nr_gpr++];
        else
            value = *ap++;

        if (param.IsOut() || !type.IsArithmetic()) {
            dp->val.p = (void*) value;
            continue;
        }
        switch (type) {
        case nsXPTType::T_I8:      dp->val.i8  = (int8_t)   value; break;
        case nsXPTType::T_I16:     dp->val.i16 = (int16_t)  value; break;
        case nsXPTType::T_I32:     dp->val.i32 = (int32_t)  value; break;
        case nsXPTType::T_I64:     dp->val.i64 = (int64_t)  value; break;
        case nsXPTType::T_U8:      dp->val.u8  = (uint8_t)  value; break;
        case nsXPTType::T_U16:     dp->val.u16 = (uint16_t) value; break;
        case nsXPTType::T_U32:     dp->val.u32 = (uint32_t) value; break;
        case nsXPTType::T_U64:     dp->val.u64 = (uint64_t) value; break;
        // Cast to uint8_t first, to remove garbage on upper 56 bits.
        case nsXPTType::T_BOOL:    dp->val.b   = (bool)(uint8_t)   value; break;
        case nsXPTType::T_CHAR:    dp->val.c   = (char)     value; break;
        case nsXPTType::T_WCHAR:   dp->val.wc  = (wchar_t)  value; break;
        default:
            NS_ERROR("bad type");
            break;
        }
    }

    nsresult result = self->mOuter->CallMethod((uint16_t) methodIndex, info, paramBuffer);

    return result;
}

extern "C" nsresult SharedStub(uint64_t,uint64_t,uint64_t,uint64_t,
                               uint64_t,uint64_t,uint64_t,uint64_t,
                               uint64_t,uint64_t*);

/*
 * a0-a7 registers parameters. Pass stub number in a first stack slot,
 * and stack args in second 
 */

#define STUB_ENTRY(n) \
nsresult nsXPTCStubBase::Stub##n(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8) \
{ \
    uint64_t a0 = (uint64_t) this; \
    return SharedStub(a0, a1, a2, a3, a4, a5, a6, a7, (uint64_t) n, &a8); \
}

#define SENTINEL_ENTRY(n) \
nsresult nsXPTCStubBase::Sentinel##n() \
{ \
    NS_ERROR("nsXPTCStubBase::Sentinel called"); \
    return NS_ERROR_NOT_IMPLEMENTED; \
}

#include "xptcstubsdef.inc"
