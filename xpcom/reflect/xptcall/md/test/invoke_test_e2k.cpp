/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>

typedef unsigned int nsresult;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned long nsXPTCVariant;

#  define NS_IMETHOD virtual nsresult
#  define NS_IMETHODIMP nsresult
#  define GPR_COUNT 8


#define STUB_ENTRY(n) \
  NS_IMETHOD callme##n(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, \
                       uint64_t, uint64_t, uint64_t) = 0;

class base {
 public:
  NS_IMETHOD ignored() = 0;
  STUB_ENTRY(1)
  STUB_ENTRY(2)
  STUB_ENTRY(3)
};

#undef STUB_ENTRY
#define STUB_ENTRY(n) \
  NS_IMETHOD callme##n(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, \
                       uint64_t, uint64_t, uint64_t) override;

class bar : public base {
 public:
  NS_IMETHOD ignored() override;
  STUB_ENTRY(1)
  STUB_ENTRY(2)
  STUB_ENTRY(3)
};

#undef STUB_ENTRY

NS_IMETHODIMP bar::ignored() { return 0; }

NS_IMETHODIMP bar::callme1(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8) {
  printf("called bar::callme1(2 args) with: %d %d\n", a1, a2);
  return 15;
}

NS_IMETHODIMP bar::callme2(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8) {
  printf("called bar::callme2(7 args) with: %d %d %d %d %d %d %d\n", a1, a2, a3, a4, a5, a6, a7);
  return 25;
}

NS_IMETHODIMP bar::callme3(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8) {
  printf("called bar::callme3(8 args) with: %d %d %d %d %d %d %d %d\n", a1, a2, a3, a4, a5, a6, a7, a8);
  return 35;
}

void docall(base* f, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4,
            uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8) {
  f->callme3(a1, a2, a3, a4, a5, a6, a7, a8);
}

static void InvokeCopyToStack(uint32_t paramCount, nsXPTCVariant *s,
                              uint64_t *stack, uint64_t *gpregs) {
    gpregs++; // Skip first reg for 'that'
    int nr = 1;
    for (uint32_t i = 0; i < paramCount; i++, s++) {
        if (nr < GPR_COUNT)
            *((uint64_t*)gpregs) = *((uint64_t*)s), gpregs++, nr++;
        else
            *((uint64_t*)stack) = *((uint64_t*)s), stack++;
    }
}

typedef nsresult (*vtable_func)(void *, uint64_t, uint64_t, uint64_t,
                                uint64_t, uint64_t, uint64_t, uint64_t);

nsresult NS_InvokeByIndex(void *that, uint32_t index, uint32_t paramCount,
                          nsXPTCVariant* params) {

  vtable_func *vtable = *reinterpret_cast<vtable_func **>(that);
  vtable_func method = vtable[index];

  uint32_t stack_slots = 0, reg_slots = 1 /*this*/;
  uint64_t *stack_space = 0;

  for (uint32_t i = 0; i < paramCount; i++)
      if (reg_slots < GPR_COUNT) reg_slots++; else stack_slots++;

  uint64_t gpregs[GPR_COUNT];

  if (stack_slots) {
      stack_slots = (stack_slots + 1) & ~1; // Stack, if used, must be 16-bytes aligned
      stack_space = reinterpret_cast<uint64_t *>(__builtin_alloca((stack_slots + GPR_COUNT) * 8));
  }

  InvokeCopyToStack(paramCount, params, stack_space, gpregs);
  return method(that, gpregs[1], gpregs[2], gpregs[3], gpregs[4], gpregs[5],
                gpregs[6], gpregs[7]);
}

/***************************************************************************/

int main() {
  nsXPTCVariant params1[8] = {1, 2, 3, 4 ,5, 6, 7, 8};
  nsXPTCVariant params2[8] = {2, 4, 6, 8, 10, 12, 14, 16};
  nsXPTCVariant params3[8] = {3, 6, 9, 12, 15, 18, 21, 24};

  base* a = new bar();

  printf("calling via C++...\n");
  docall(a, 3, 6, 9, 12, 15, 18, 21, 24);

  printf("calling via Invoke...\n");
  NS_InvokeByIndex(a, 1, 2, params1);
  NS_InvokeByIndex(a, 2, 7, params2);
  NS_InvokeByIndex(a, 3, 8, params3);

  return 0;
}
