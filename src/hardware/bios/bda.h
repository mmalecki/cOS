/* 
 * File:   bda.h
 * Author: Maciej
 *
 * Created on 15 sierpień 2010, 21:13
 */

#ifndef _BDA_H
  #define	_BDA_H

  #ifdef	__cplusplus
extern "C" {
  #endif

#include "types.h"

#define BDA_ADDRESS      (0x400 + 0xC0000000)
#define BDA_EBDA_ADDRESS BDA_ADDRESS + 0x0E
#define BDA_MEM_SIZE     BDA_ADDRESS + 0x13

/*
 * Many of these addresses come from http://wiki.osdev.org/Memory_Map_(x86)
 * and http://stanislavs.org/helppc/bios_data_area.html
 */

void* bda_get_ebda();
uint32 bda_get_mem_size();

  #ifdef	__cplusplus
}
  #endif

#endif	/* _BDA_H */

