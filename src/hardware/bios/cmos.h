#ifndef _CMOS_H
#define	_CMOS_H

#include "types.h"

#define CMOS_REGISTRY_SEL_PORT 0x70
#define CMOS_DATA_PORT         0x71

uint8 cmos_read(uint8 reg);
void cmos_write(uint8 reg, uint8 val);
uint8 cmos_reads(uint8 reg); // cmos_reads doesn't disable interrupts
void cmos_writes(uint8 reg, uint8 val);

#endif

