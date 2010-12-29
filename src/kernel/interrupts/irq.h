#ifndef _IRQ_H
#define	_IRQ_H

#include "types.h"

#define IRQ0_DEFAULT_OFFSET 0x20
#define IRQ1_DEFAULT_OFFSET 0x28

#define IRQ_PIC0         0x20
#define IRQ_PIC0_COMMAND IRQ_PIC0
#define IRQ_PIC0_DATA    IRQ_PIC0 + 1

#define IRQ_PIC1         0xA0
#define IRQ_PIC1_COMMAND IRQ_PIC1
#define IRQ_PIC1_DATA    IRQ_PIC1 + 1

#define IRQ_PIC_INIT 0x11
#define IRQ_EOI      0x20

#define IRQ_PIC_8086_MODE 0x01
// I suppose I won't use any other, so I won't define them. Do it if you want to.

void irq_init();
bool irq_is_irq(uint8 int_n);

#endif

