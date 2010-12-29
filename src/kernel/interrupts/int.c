#include "int.h"
#include "io.h"

void int_disable() {
  asm volatile("cli");
}

void int_enable() {
  asm volatile("sti");
}

void int_nmi_disable() {
  outb(INT_NMI_CONTROL, inb(INT_NMI_CONTROL) | INT_NMI_BIT);
}

void int_nmi_enable() {
  outb(INT_NMI_CONTROL, inb(INT_NMI_CONTROL) & (~INT_NMI_BIT));
}

