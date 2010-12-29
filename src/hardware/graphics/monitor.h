#ifndef _MONITOR_H
#define _MONITOR_H

#include "types.h"

#define COLOUR_MEM     (0xB8000 + 0xC0000000)
#define MONOCHROME_MEM (0xA8000 + 0xC0000000)

bool monitor_is_colour();
unsigned char* monitor_get_mem();

#endif
