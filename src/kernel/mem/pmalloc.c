#include "pmalloc.h"
#include "types.h"

extern uint32 end;
uint32 address = (uint32) &end;

void* _pmalloc(size_t sz, int align, uint32* phys) {
  if (align && (address & (align - 1))) {
    address &= ~(align - 1);
    address += align;
  }
  uint32 res = address;
  if (phys)
    *phys = res;
  address += sz;
  return (void*) res;
}

void* pmalloc(size_t sz) {
  return _pmalloc(sz, FALSE, NULL);
}
