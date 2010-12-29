#ifndef IDT_H
#define	IDT_H

#include "types.h"

#define IDT_TASK_GATE_32 1 | 1 << 2
#define IDT_INT_GATE_16  1 << 1 | 1 << 2
#define IDT_INT_GATE_32  1 << 1 | 1 << 2 | 1 << 3
#define IDT_TRAP_GATE_16 1 | 1 << 1 | 1 << 2
#define IDT_TRAP_GATE_32 1 | 1 << 1 | 1 << 2 | 1 << 3

#define IDT_RING_0 0
#define IDT_RING_1 1 << 5
#define IDT_RING_2 1 << 6
#define IDT_RING_3 1 << 5 | 1 << 6

typedef struct idt_entry {
  uint16 base_low;
  uint16 selector;
  uint8  zero;  // should be always set to 0
  uint8  flags;
  uint16 base_high;
} __attribute__((packed)) idt_entry_t, *pidt_entry_t;

typedef struct idt_pointer {
  uint16 limit;
  uint32 base;
} __attribute__((packed)) idt_pointer_t, *pidt_pointer_t;

typedef struct registers {
  uint32 ds;
  uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; 
  uint32 int_no, err_code;    
  uint32 eip, cs, eflags, useresp, ss; 
} registers_t, *pregister_t;

typedef void (*interrupt_handler_t) (registers_t);

void idt_init();
void idt_fill_entry(int entry, uint32 handler, uint8 ring, uint8 type);
void idt_set_handler(int entry, interrupt_handler_t handler);
/*
 * Please note, that you have a possibility of setting external handlers for
 * each of 256 interrupts, though basic ISRs are implemented only from 0 to 31,
 * and externals handlers for those will be called only.
 *
 * But, hey, don't get nervous! There is always another way! In fact, in this case,
 * there are two ways:
 *
 *  * set your own interrupt handler using idt_fill_entry, but this is uncomfortable,
 *    since you have to use ASM to push, pop registers and iret. It sucks.
 *  * use IRQ bult-in mechanisms. I can't tell any details, because I haven't
 *    written them yet. But I hope you're gonna like it.
 *
 * Oh, don't also forget, that idt_set_handler will overwrite previous handler
 * for the same entry without any notice.
 */

#endif

