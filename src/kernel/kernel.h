#ifndef _KERNEL_H
#define _KERNEL_H

#include "boot/multiboot.h"
#include "types.h"

void kmain(multiboot_info_t* mbi);

#endif
