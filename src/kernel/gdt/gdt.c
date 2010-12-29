#include "gdt.h"
#include "types.h"
#include "kernel/kernel.h"

gdt_entry_t   gdt_entries[5];
gdt_pointer_t gdt_pointer;

extern void gdt_flush(uint32);

void gdt_init() {
  gdt_pointer.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gdt_pointer.base  = (uint32) &gdt_entries;
  gdt_set_entry(0, 0, 0, 0, 0);
  gdt_fill_entry(1, 0, 0xFFFFFFFF, GDT_RING_0, GDT_CODE_SEGMENT);
  gdt_fill_entry(2, 0, 0xFFFFFFFF, GDT_RING_0, GDT_DATA_SEGMENT);
  gdt_fill_entry(3, 0, 0xFFFFFFFF, GDT_RING_3, GDT_CODE_SEGMENT);
  gdt_fill_entry(4, 0, 0xFFFFFFFF, GDT_RING_3, GDT_DATA_SEGMENT);
  gdt_flush((uint32) &gdt_pointer);
}

void gdt_fill_entry(int entry, uint32 base, uint32 limit, uint8 ring,
   uint8 segment_type) {
  gdt_entries[entry].base_low    = base & 0xFFFF;
  gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
  gdt_entries[entry].base_middle = (base >> 24) & 0xFF;
  gdt_entries[entry].limit_low   = limit & 0xFFFF;
  gdt_entries[entry].granularity = ((limit >> 16) & 0x0F) | GDT_GRANULARITY_1KB | GDT_OPERAND_32;
  gdt_entries[entry].access      = ring | segment_type | 1 << 1 | 1 << 4 | GDT_SEGMENT_PRESENT;
}

void gdt_set_entry(int entry, uint32 base, uint32 limit, uint8 access,
   uint8 granularity) {
  gdt_entries[entry].base_low    = base & 0xFFFF;
  gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
  gdt_entries[entry].base_middle = (base >> 24) & 0xFF;
  gdt_entries[entry].limit_low   = limit & 0xFFFF;
  gdt_entries[entry].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
  gdt_entries[entry].access      = access;
}