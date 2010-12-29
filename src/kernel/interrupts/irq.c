#include "irq.h"
#include "idt.h"
#include "types.h"
#include "io.h"

uint8 irq0_offset = IRQ0_DEFAULT_OFFSET;
uint8 irq1_offset = IRQ1_DEFAULT_OFFSET;

interrupt_handler_t irq_handlers[16];

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void irq_init() {
  uint8 pic0_icw4, pic1_icw4;
  pic0_icw4 = inb(IRQ_PIC0_DATA);
  pic1_icw4 = inb(IRQ_PIC1_DATA);
  outb(IRQ_PIC0_COMMAND, IRQ_PIC_INIT);
  outb(IRQ_PIC1_COMMAND, IRQ_PIC_INIT);
  outb(IRQ_PIC0_DATA, irq0_offset);
  outb(IRQ_PIC1_DATA, irq1_offset);
  outb(IRQ_PIC0_DATA, 0x04);
  outb(IRQ_PIC1_DATA, 0x02);
  outb(IRQ_PIC0_DATA, IRQ_PIC_8086_MODE);
  outb(IRQ_PIC1_DATA, IRQ_PIC_8086_MODE);
  outb(IRQ_PIC0_DATA, pic0_icw4);
  outb(IRQ_PIC1_DATA, pic1_icw4);

  idt_fill_entry(irq0_offset,      (uint32) irq0,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq0_offset + 1,  (uint32) irq1,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq0_offset + 2,  (uint32) irq2,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq0_offset + 3,  (uint32) irq3,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq0_offset + 4,  (uint32) irq4,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq0_offset + 5,  (uint32) irq5,  IDT_RING_0, IDT_TRAP_GATE_32);
  idt_fill_entry(irq0_offset + 6,  (uint32) irq6,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq0_offset + 7,  (uint32) irq7,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 8,  (uint32) irq8,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 9,  (uint32) irq9,  IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 10, (uint32) irq10, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 11, (uint32) irq11, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 12, (uint32) irq12, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 13, (uint32) irq13, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 14, (uint32) irq14, IDT_RING_0, IDT_INT_GATE_32);
  idt_fill_entry(irq1_offset + 15, (uint32) irq15, IDT_RING_0, IDT_INT_GATE_32);
}

bool irq_is_irq(uint8 int_n) {
  return ((int_n >= irq0_offset && int_n < (irq0_offset + 8)) ||
          (int_n >= irq1_offset && int_n < (irq1_offset + 8))) ? TRUE : FALSE;
}

void irq_handler(registers_t registers) {
  if (irq_handlers[registers.err_code] != NULL)
    irq_handlers[registers.err_code](registers);

  if (registers.err_code > 7)
    outb(IRQ_PIC1_COMMAND, IRQ_EOI);
  outb(IRQ_PIC0_COMMAND, IRQ_EOI);
}
