#include "cmos.h"
#include "types.h"
#include "kernel/interrupts/int.h"
#include "io.h"

uint8 cmos_read(uint8 reg) {
  int_disable();
  outb(CMOS_REGISTRY_SEL_PORT, INT_NMI_BIT | reg);
  return inb(CMOS_DATA_PORT);
  int_enable();
}

void cmos_write(uint8 reg, uint8 val) {
  int_disable();
  outb(CMOS_REGISTRY_SEL_PORT, INT_NMI_BIT | reg);
  outb(CMOS_DATA_PORT, val);
  int_enable();
}

uint8 cmos_reads(uint8 reg) {
  outb(CMOS_REGISTRY_SEL_PORT, INT_NMI_BIT | reg);
  return inb(CMOS_DATA_PORT);
}

void cmos_writes(uint8 reg, uint8 val) {
  outb(CMOS_REGISTRY_SEL_PORT, INT_NMI_BIT | reg);
  outb(CMOS_DATA_PORT, val);
}