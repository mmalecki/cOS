#include <string.h>
#include "hardware/cpu/cpu.h"

extern uint32 cpu_ext_mask;

void* memmove(void* destination, const void* source, size_t num) {
  size_t n = num;
  const char* src = source;
  char* dest = destination;
  if (dest == src)
    return destination;
  else if (dest > src && dest < (src + num)) {
    while (num-- != 0)
      *(dest + num) = *(src + num);
  }
  else {
    for (n = 0; n < num; n++)
      *(dest + n) = *(src + n);
  }
  return destination;
}

void* memcpy(register void* destination, register const void* src, register size_t sz) {
  void* dest = destination;
  #ifdef __SSE2__
  if (CPU_EXT(SSE2)) {
    // SSE way, freaking fast
    register char r = 0x10 - (((long unsigned int) dest) & 0xF);
    if (r != 0x10) {
      asm("rep movsb" : : "c" (r), "S" (src), "D" (dest) :);
      dest += r;
      src += r;
      sz -= r;
    }
    for (; sz >= 128; sz -= 128) {
      asm(
        "movdqa 0(%0),   %%xmm0  \n\t"
        "movdqa 16(%0),  %%xmm1  \n\t"
        "movdqa 32(%0),  %%xmm2  \n\t"
        "movdqa 48(%0),  %%xmm3  \n\t"
        "movdqa 64(%0),  %%xmm4  \n\t"
        "movdqa 80(%0),  %%xmm5  \n\t"
        "movdqa 96(%0),  %%xmm6  \n\t"
        "movdqa 112(%0), %%xmm7  \n\t"
        "movdqa %%xmm0,  0(%1)   \n\t"
        "movdqa %%xmm1,  16(%1)  \n\t"
        "movdqa %%xmm2,  32(%1)  \n\t"
        "movdqa %%xmm3,  48(%1)  \n\t"
        "movdqa %%xmm4,  64(%1)  \n\t"
        "movdqa %%xmm5,  80(%1)  \n\t"
        "movdqa %%xmm6,  96(%1)  \n\t"
        "movdqa %%xmm7,  112(%1) \n\t"
      : : "S" (src), "D" (dest) :);
      src += 128;
      dest += 128;
    }
    if (sz > 0)
      --sz; // I don't really know why, otherwise it won't work
    asm("rep movsb" : : "c" (sz), "S" (src), "D" (dest) :);
  }
  else {
  #endif
    // non-SSE way
    asm("rep movsb" : : "c" (sz), "S" (src), "D" (dest) :);
  #ifdef __SSE2__
  }
  #endif
  return destination;
}

char* strcat(char* destination, const char* source) {
  while (*(++destination) != 0);
  while ((*destination++ = *source++) != 0);
  return destination;
}

int strcmp(const char* str1, const char* str2) {
  // TODO: MMX/SSE!
  for (; *str1 && *str2; str1++, str2++) {
    if (*str1 > *str2)
      return 1;
    else if (*str1 < *str2)
      return -1;
  }
  
  if (*str1 == 0 && *str2 == 0)
    return 0;
  else
    return (*str1 > *str2) ? 1 : -1;
}

int strncmp(const char* str1, const char* str2, size_t num) {
  // TODO: MMX/SSE!
  size_t i;
  char c1, c2;
  for (i = 0; i < num; i++) {
    c1 = *(str1++);
    c2 = *(str2++);
    if (c1 > c2)
      return 1;
    else if (c1 < c2)
      return -1;
    else if (c1 == c2 && c1 == 0)
      return 0;
  }
  return 0;
}

char* strchr(const char* str, int character) {
  do {
    if (*str == character)
      return (char*) str;
  } while (*(++str) != 0);
  return NULL;
}

char* strstr(const char* str1, const char* str2) {
  int l = strlen(str2);
  char* pos;
  while ((pos = strchr(str1, *str2)) != NULL) {
    if (strncmp(pos, str2, l) == 0)
      return pos;
    str1++;
  }
  return NULL;
}