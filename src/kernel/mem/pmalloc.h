#ifndef _PMALLOC_H
#define	_PMALLOC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"

#ifndef SIZE_T
  typedef unsigned int size_t;
  #define SIZE_T
#endif

void* _pmalloc(size_t sz, int align, uint32* phys);
void* pmalloc(size_t sz);
void pfree(size_t sz);

#ifdef	__cplusplus
}
#endif

#endif	/* _PMALLOC_H */

