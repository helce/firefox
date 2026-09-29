/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>

typedef unsigned long nsresult;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned nsXPCVariant;

#  define NS_IMETHOD virtual nsresult
#  define NS_IMETHODIMP nsresult

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

class baz : public base {
 public:
  NS_IMETHOD ignored() override;
  STUB_ENTRY(1)
  STUB_ENTRY(2)
  STUB_ENTRY(3)
  void setbase(base* f) { other = f; }

  base* other;
};

#undef STUB_ENTRY

NS_IMETHODIMP baz::ignored() { return 0; }

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

extern "C" nsresult PrepareAndDispatch(baz* self, uint32_t methodIndex, uint64_t* args, uint64_t* gpregs) {
  base* a = self->other;
  gpregs++; // Skip self
  int a1 = (int)*(gpregs++);
  int a2 = (int)*(gpregs++);
  int a3 = (int)*(gpregs++);
  int a4 = (int)*(gpregs++);
  int a5 = (int)*(gpregs++);
  int a6 = (int)*(gpregs++);
  int a7 = (int)*(gpregs++);
  int a8 = (int)*(args);

  switch (methodIndex) {
    case 1:
      a->callme1(a1, a2, 0, 0, 0, 0, 0 ,0);
      break;
    case 2:
      a->callme2(a1, a2, a3, a4, a5, a6, a7, 0);
      break;
    case 3:
      a->callme3(a1, a2, a3, a4, a5, a6, a7, a8);
      break;
  }
  return 1;
}

extern "C" nsresult SharedStub(uint64_t,uint64_t,uint64_t,uint64_t,
                               uint64_t,uint64_t,uint64_t,uint64_t,
                               uint64_t,uint64_t*);

#define STUB_ENTRY(n) \
nsresult baz::callme##n(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8) \
{ \
  uint64_t a0 = (uint64_t) this; \
  return SharedStub(a0, a1, a2, a3, a4, a5, a6, a7, (uint64_t) n, &a8); \
}

/***************************************************************************/

STUB_ENTRY(1)
STUB_ENTRY(2)
STUB_ENTRY(3)

int main() {
  base* a = new bar();
  baz* b = new baz();

  /* here we make the global 'check for alloc failure' checker happy */
  if (!a || !b) return 1;

  base* c = (base*)b;

  b->setbase(a);
  c->callme1(1, 2, 3, 4, 5, 6, 7, 8);
  c->callme2(2, 4, 6, 8, 10, 12, 14, 16);
  c->callme3(3, 6, 9, 12, 15, 18, 21, 24);

  return 0;
}
