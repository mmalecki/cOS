/* 
 * File:   apic.h
 * Author: Maciej
 *
 * Created on 31 sierpień 2010, 19:09
 */

#ifndef _APIC_H
  #define	_APIC_H

  #ifdef	__cplusplus
extern "C" {
  #endif

#define APIC_IMCR_SEL_PORT 0x22
#define APIC_IMCR_SEL_VAL  0x70
#define APIC_IMCR_PORT     0x23

void apic_init();

  #ifdef	__cplusplus
}
  #endif

#endif	/* _APIC_H */

