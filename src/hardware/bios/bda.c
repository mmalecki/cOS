#include "bda.h"
#include "types.h"

void* bda_get_ebda() {
  /*
   * This will work with good probability. If it doesn't... Well, oops.
   * To be serious: there's no way we can check if EBDA is really there, since
   * it's specific to each BIOS.
   */
  return (void*) (*((uint16*) BDA_EBDA_ADDRESS) << 4);
}

uint32 bda_get_mem_size() {
  uint16 sz_kb = *((uint16*) BDA_MEM_SIZE);
  return sz_kb * 1024;
}