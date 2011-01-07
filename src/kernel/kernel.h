#ifndef _KERNEL_H
#define _KERNEL_H

#include "boot/multiboot.h"
#include "types.h"


//const uint32 KERNEL_VIRTUAL_BASE = 0xC0000000;

void kmain(multiboot_info_t* mbi);

#endif
