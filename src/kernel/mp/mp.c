#include "mp.h"
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "hardware/bios/bda.h"
#include "kernel/screen.h"

mp_fp_t*               mp_floating_pointer = NULL;
mp_conf_table_t*       mp_conf_table       = NULL;
mp_processor_entry_t*  mp_processors       = NULL;
int                    mp_processor_count  = 0;
mp_bus_entry_t*        mp_buses            = NULL;
int                    mp_bus_count        = 0;
mp_io_apic_entry_t*    mp_io_apics         = NULL;
int                    mp_io_apic_count    = 0;

void mp_get_info();

mp_fp_t* mp_find_fp(void* addr, uint32 len) {
  void* p;
  void* e = len + addr;
  for (p = addr; p < e; p += sizeof(mp_fp_t)) {
    if (strncmp((const char*) p, "_MP_", 4) == 0) 
      return (mp_fp_t*) p;
  }
  return NULL;
}

mp_fp_t* mp_get_fp() {
  /*
   * OK, so this is our search plan:
   *   0) first kilobyte of EBDA (Extended BIOS Data Area),
   *   1) last kilobyte of memory,
   *   2) area from 0xE0000 to 0xFFFFF.
   *
   * Finally, 0x00000, if we fail ;).
   */
  if (mp_floating_pointer == NULL) {
    mp_fp_t* optr = NULL;
    if ((optr = mp_find_fp(bda_get_ebda(), 1024)) != NULL);
    else if ((optr = mp_find_fp(
       (void*) bda_get_mem_size() - 1024, 1024)) != NULL);
    else
      optr = mp_find_fp((void*) 0xE0000, 0x1FFFF);
    mp_floating_pointer = optr;
    return optr;
  }
  else
    return mp_floating_pointer;
}

mp_conf_table_t* mp_get_conf_table() {
  if (mp_conf_table == NULL) {
    if (mp_floating_pointer == NULL)
      mp_get_fp();

    if (mp_floating_pointer->physical_address) {
      mp_conf_table = (mp_conf_table_t*) mp_floating_pointer->physical_address;
      /*
       * In fact, multiproc.c has got no idea of paging. But we'll say that
       * it assumes that whole memory is identity-mapped. It just sounds better.
       * The second thing is, that *here* (in a mean of time) almost whole memory
       * *should* be identity-mapped.
       */
      if (strncmp(mp_conf_table->signature, "PCMP", 4) == 0)
        return mp_conf_table;
      else {
        mp_conf_table = NULL;
        return NULL;
      }
    }
    else
      return NULL;
  }
  else
    return mp_conf_table;
}

mp_processor_entry_t* mp_get_processors() {
  if (mp_processors == NULL) 
    mp_get_info();
  return mp_processors;
}

uint mp_get_buses_count() {
  if (mp_buses == NULL)
    mp_get_info();
  return mp_bus_count;
}

mp_io_apic_entry_t* mp_get_io_apics() {
  if (mp_io_apics == NULL) 
    mp_get_info();
  return mp_io_apics;
}

void mp_get_info() {
  mp_processor_count = 0;
  mp_bus_count       = 0;
  mp_io_apic_count   = 0;

  if (mp_get_conf_table() == NULL) {
    mp_processor_count = -1;
    mp_bus_count       = -1;
    mp_io_apic_count   = -1;
    return;
  }

  int sum = 0;

  uint8* ct = (uint8*) mp_get_conf_table();
  ct += sizeof(mp_conf_table_t);

  if (*ct == 0) {
    mp_processors = (mp_processor_entry_t*) ct;
    while (*ct == 0 && sum < mp_conf_table->entry_count) {
      ++mp_processor_count;
      ++sum;
      ct += sizeof(mp_processor_entry_t);
    }
  }

  if (*ct == 1) {
    mp_buses = (mp_bus_entry_t*) ct;
    while (*ct == 1 && sum < mp_conf_table->entry_count) {
      ++mp_bus_count;
      ++sum;
      ct += sizeof(mp_bus_entry_t);
    }
  }

  if (*ct == 2) {
    mp_io_apics = (mp_io_apic_entry_t*) ct;
    while (*ct == 2 && sum < mp_conf_table->entry_count) {
      ++mp_io_apic_count;
      ++sum;
      ct += sizeof(mp_io_apic_entry_t);
    }
  }
}

void mp_init() {
  mp_get_fp();
  mp_get_conf_table();
  mp_get_info();
  char buf[256];
  sprintf(buf,
    "\n   MultiProcessor information dump:\n"
    "    OEM ID:               %8s\n"
    "    Product ID:           %12s\n"
    "    Number of processors: %i\n"
    "    Number of buses:      %i\n"
    "    Number of I/O APICs:  %i\n",
    mp_conf_table->oem_id, mp_conf_table->product_id,
    mp_processor_count, mp_bus_count, mp_io_apic_count);
  kputs(buf);

  if (mp_processor_count >= 64)
    kputs("   Hey, steroids aren't good for CPUs!\n");
  else if (mp_processor_count >= 16)
    kputs("   Oh, my Flying Spaghetti Monster, this kicks ass!\n");
  else if (mp_processor_count > 4)
    kputs("   Wow! What a beast! It will be a great pleasure to work on this hardware!\n");
}
