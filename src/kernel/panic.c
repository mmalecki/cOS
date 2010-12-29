#include "panic.h"
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "kernel/screen.h"

void _kpanic(char* msg, char* file, int line) {
  // In future we will terminate all grasoles here.
  char buf[1024];
  sprintf(buf, 
    "Ooops! Our kernel has overdosed DMT!\n\n"
    " Kernel is freaking out with message:\n  %s\n which was caused by file:\n"
    "  %s, at line %i\n\n Will now try to dump registers.\n", msg, file, line);
  kputs(buf);
  uint32 eax, ebx, ecx, edx, esi, edi, ebp, esp, cr0, cr2, cr3, cr4;
  asm volatile(
    "nop \t\n"
    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx), "=S" (esi), "=D" (edi) : :);
  asm volatile(
    "mov %%ebp, %0 \n\t"
    "mov %%esp, %1 \n\t"
    "mov %%cr0, %2 \n\t"
    "mov %%cr2, %3 \n\t"
    "mov %%cr3, %4 \n\t"
    "mov %%cr4, %5 \n\t"
    : "=r" (ebp), "=r" (esp), "=r" (cr0), "=r" (cr2), "=r" (cr3), "=r" (cr4) : : );
  
  memclr(buf, 1024);
  sprintf(buf,
    " Registers dump:\n"
    "  eax = 0x%8x, ebx = 0x%8x, ecx = 0x%8x, edx = 0x%8x\n"
    "  esi = 0x%8x, edi = 0x%8x, ebp = 0x%8x, esp = 0x%8x\n"
    "  cr0 = 0x%8x, cr2 = 0x%8x, cr3 = 0x%8x, cr4 = 0x%8x\n",
    eax, ebx, ecx, edx, esi, edi, ebp, esp, cr0, cr2, cr3, cr4);
  kputs(buf);
  asm("hlt");
}
