#ifndef _INTERRUPTS_H
#define	_INTERRUPTS_H

#define INT_NMI_BIT     1 << 7
#define INT_NMI_CONTROL 0x70

void int_disable();
void int_enable();
void int_nmi_disable();
void int_nmi_enable();

#endif	

