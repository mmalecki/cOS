/*!
 * \file   paging.h
 * \author Maciej Małecki
 * \brief  Paging functions
 *
 * paging.h defines functions and types used by kernel to manage paging, which
 * is default way of memory management in this OS.
 */


#ifndef _PAGING_H
#define	_PAGING_H

#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define PAGE_PRESENT           1

#define PAGE_READWRITE         (1 << 1)
#define PAGE_READONLY          0

#define PAGE_ACCESS_USER       (1 << 2)
#define PAGE_ACCESS_SUPERVISOR 0

#define PAGE_WRITETHROUGH      (1 << 3)
#define PAGE_WRITEBACK         0

#define PAGE_CACHE_DISABLE     (1 << 4)
#define PAGE_CACHE_ENABLED     0

#define PAGE_ACCESSED          (1 << 5)

#define PAGE_SIZE_4KB          0
#define PAGE_SIZE_4MB          (1 << 7)

#define PAGE_ADDR(addr)        ((addr) << 12)

#define PAGE_DEFAULT_MASK      PAGE_READWRITE | PAGE_ACCESS_USER | PAGE_WRITEBACK \
                               | PAGE_CACHE_ENABLED | PAGE_PRESENT | PAGE_SIZE_4KB
    //

void paging_create_empty_directory(uint32* dir);
uint32* paging_create_page_table(uint32 bitmask, uint32 startaddr);
void paging_init();
void paging_switch_directory(void* dir);
void paging_enable();
void paging_enable_pae();

#ifdef	__cplusplus
}
#endif

#endif	/* _PAGING_H */

