#include "idt.h"
#include <string.h>
#include <stdio.h>
#include "kernel/screen.h"
#include "irq.h"

idt_entry_t         idt_entries[256];
idt_pointer_t       idt_pointer;
interrupt_handler_t int_handlers[256];

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void idt_flush(uint32);

void isr_handler(registers_t registers);

void idt_init() {
  idt_pointer.limit = (sizeof(idt_entry_t) * 256) - 1;
  idt_pointer.base  = (uint32) &idt_entries;
  memclr(&idt_entries, sizeof(idt_entry_t) * 256);
  memclr(&int_handlers, sizeof(interrupt_handler_t) * 256);

  idt_fill_entry(0, (uint32) isr0, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(1, (uint32) isr1, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(2, (uint32) isr2, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(3, (uint32) isr3, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(4, (uint32) isr4, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(5, (uint32) isr5, IDT_RING_0, IDT_TRAP_GATE_32);
  idt_fill_entry(6, (uint32) isr6, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(7, (uint32) isr7, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(8, (uint32) isr8, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(9, (uint32) isr9, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(10, (uint32) isr10, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(11, (uint32) isr11, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(12, (uint32) isr12, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(13, (uint32) isr13, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(14, (uint32) isr14, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(15, (uint32) isr15, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(16, (uint32) isr16, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(17, (uint32) isr17, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(18, (uint32) isr18, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(19, (uint32) isr19, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(20, (uint32) isr20, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(21, (uint32) isr21, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(22, (uint32) isr22, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(23, (uint32) isr23, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(24, (uint32) isr24, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(25, (uint32) isr25, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(26, (uint32) isr26, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(27, (uint32) isr27, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(28, (uint32) isr28, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(29, (uint32) isr29, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(30, (uint32) isr30, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(31, (uint32) isr31, IDT_RING_0, IDT_INT_GATE_32);

  idt_flush((uint32) &idt_pointer);
}

void idt_fill_entry(int entry, uint32 handler, uint8 ring, uint8 type) {
  idt_entries[entry].base_low  = handler & 0xFFFF;
  idt_entries[entry].base_high = (handler >> 16) & 0xFFFF;
  idt_entries[entry].selector  = 0x08;
  /*
   * Selector is somehow a pointer to a descriptor table.
   * It consists of 3 section: bits 0 and 1 select a ring that descriptor
   * refers to, bit 2 is 0 for GDT and 1 for LDT, bits 3 .. 15 represent
   * a table index.
   *
   * In here we will hardcode 0x08, which will point to ring 0, GDT and first
   * index. That's ring 0 code section.
   */
  idt_entries[entry].zero = 0; // it should be already set by memset, but... ;)
  idt_entries[entry].flags = ring | type | 1 << 7;
}

void isr_handler(registers_t registers) {
  char buf[64];
  sprintf(buf, "An interrupt #%i raised.\n", registers.int_no);
  screen_puts(buf);
  asm("hlt");
  if (int_handlers[registers.int_no] != NULL)
    int_handlers[registers.int_no](registers);
}

void idt_set_handler(int entry, interrupt_handler_t handler) {
  int_handlers[entry] = handler;
}