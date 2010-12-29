#include "monitor.h"
#include "types.h"

bool monitor_is_colour() {
  char t = (*(unsigned short int*) (0x410 + 0xC0000000)) & 0x30;
  return t == 0 || t == 0x20;
}

unsigned char* monitor_get_mem() {
  return (unsigned char*) ((monitor_is_colour()) ? COLOUR_MEM : MONOCHROME_MEM);
}
