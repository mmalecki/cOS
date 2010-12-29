#ifndef GDT_H
#define	GDT_H

#include "types.h"

/*
 * This file (along with gdt.s and gdt.c) takes care of initializing GDT table,
 * which quoting Wiki is:
 *
 * The Global Descriptor Table or GDT is a data structure used by Intel x86-family
 * processors starting with the 80286 in order to define the characteristics of
 * the various memory areas used during program execution, for example the base address,
 * the size and access privileges like executability and writability. These memory areas
 * are called segments in Intel terminology.
 *
 * Which I rather like to call God Damn it Table or Global Demolition Table,
 * to properly convey the time spent on it.
 */

// These are access flags...
#define GDT_SEGMENT_PRESENT 1 << 7
#define GDT_RING_3          1 << 6 | 1 << 5
#define GDT_RING_2          1 << 6
#define GDT_RING_1          1 << 5
#define GDT_RING_0          0
#define GDT_CODE_SEGMENT    1 << 3
#define GDT_DATA_SEGMENT    0

// ... and these are granularity flags.
#define GDT_OPERAND_16      0
#define GDT_OPERAND_32      1 << 6
#define GDT_GRANULARITY_1B  0
#define GDT_GRANULARITY_1KB 1 << 7

typedef struct gdt_entry {
  uint16 limit_low; // you wonder where limit_high is? somehow in granularity (lowest 4 bits)
  uint16 base_low;
  uint8  base_middle;
  uint8  access;
  uint8  granularity;
  uint8  base_high;
} __attribute__((packed)) gdt_entry_t, *pgdtentry_t;

typedef struct gdt_pointer {
  uint16 limit;
  uint32 base;
} __attribute__((packed)) gdt_pointer_t,  *pgdt_pointer_t;

void gdt_init();
void gdt_fill_entry(int entry, uint32 base, uint32 limit, uint8 ring,
   uint8 segment_type);
void gdt_set_entry(int entry, uint32 base, uint32 limit, uint8 access,
   uint8 granularity);

#endif	

