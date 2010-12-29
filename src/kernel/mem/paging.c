#include "paging.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pmalloc.h"
#include "types.h"
#include "macros.h"
#include "boot/multiboot.h"
#include "kernel/screen.h"
#include "hardware/cpu/cpu.h"

uint32* current_directory;
uint32 kernel_directory[1024] __attribute__ ((aligned (4096)));


#ifdef WITH_PAE
uint64* current_page_dir_ptr;
uint64* kernel_page_dir_ptr;
#endif

extern uint32 cpu_ext_mask;

void paging_pae_enable();
uint64* paging_pae_create_directory_ptr();
uint64* paging_pae_create_directory(uint64 start_addr, uint32 bitmask, uint32 pt_bitmask);
uint64* paging_pae_create_page_table(uint64 start_addr, uint32 bitmask);

void paging_create_directory(uint32* dir) {
  dir = _pmalloc(sizeof(uint32) * 1024, 0x1000, NULL);
  memclr(dir, sizeof(uint32) * 1024);
  int i;
  for (i = 0; i < 1024; i++) {
    uint32* page_table = _pmalloc(1024 * sizeof(uint32), 0x1000, NULL);
    memclr(page_table, 1024 * sizeof(uint32));
    dir[i] = (uint32) page_table;
  }
}

void paging_create_kernel_directory() {
  int i;
  #ifdef WITH_PAE
  if (CPU_EXT(PAE)) {
    kernel_page_dir_ptr = _pmalloc(sizeof(uint64) * 4, 0x1000, NULL);
    uint64 addr = 0;
    for (i = 0; i < 4; i++) {
      kernel_page_dir_ptr[i] = (uint32) paging_pae_create_directory(addr,
         PAGE_DEFAULT_MASK, PAGE_DEFAULT_MASK) | 1;
      addr += 512 * 512 * 4096;
    }
  }
  else {
  #endif
    //kernel_directory = _pmalloc(sizeof(uint32) * 1024, 0x1000, NULL);
    memclr(kernel_directory, sizeof(uint32) * 1024);
    
    uint32 addr, paddr;
    uint32* page_table;

    page_table = paging_create_page_table(PAGE_DEFAULT_MASK, 0);
    paddr = (uint32) page_table - 0xC0000000; // doesn't fucking work in other way!
    kernel_directory[0] = PAGE_DEFAULT_MASK | paddr;
    
    addr = 0;
    for (i = 768; i < 1023; i++) {
      page_table = paging_create_page_table(PAGE_DEFAULT_MASK, addr);
      paddr = (uint32) page_table - 0xC0000000;
      kernel_directory[i] = PAGE_DEFAULT_MASK | paddr;
      addr += 4096 * 1024;
    }

    paddr = (uint32) kernel_directory - 0xC0000000;
    page_table = paging_create_page_table(PAGE_DEFAULT_MASK, paddr);
    paddr = (uint32) page_table - 0xC0000000;
    kernel_directory[1023] = PAGE_DEFAULT_MASK | paddr;
    
    /*
     * As you maybe noticed, last KB is mapped to kernel_directory, so we'll to
     * easily manipulate it, if something bad happends.
     */
  #ifdef WITH_PAE
  }
  #endif
}

uint32* paging_create_page_table(uint32 bitmask, uint32 startaddr) {
  uint32* page_table = _pmalloc(1024 * sizeof(uint32), 0x1000, NULL);
  memclr(page_table, 1024 * sizeof(uint32));

  int i;
  for (i = 0; i < 1024; i++) {
    page_table[i] = bitmask | startaddr;
    startaddr += 4096;
  }
  return page_table;
}

void paging_init() {
  paging_create_kernel_directory();
  #ifdef WITH_PAE
  if (CPU_EXT(PAE)) {
    paging_switch_directory(kernel_page_dir_ptr);
    paging_pae_enable();
  }
  else {
  #endif
    uint32 a = (uint32) kernel_directory;
    a -= 0xC0000000;
    asm volatile("mov %0,  %%cr3 \n\t" : : "r" (a));
    
    //paging_switch_directory(kernel_directory);
  #ifdef WITH_PAE
  }
  #endif
  //paging_enable();
}

void paging_switch_directory(void* dir) {
  current_directory = dir;
  asm volatile("mov %0, %%cr3" : : "r" (dir));
}

void paging_enable() {
  asm volatile(
    "mov %%cr0,       %%eax \n\t"
    "or  $0x80000000, %%eax \n\t"
    "mov %%eax,       %%cr0 \n\t"
  : : : "eax");
}

uint8 paging_map_mem(void* dir, uint32 start_addr, uint32 end_addr, uint32 phys_addr) {
  int dirn, tabn;
  
  if ((start_addr & 0xfff) || (end_addr & 0xfff))
    return 1;
  #ifdef WITH_PAE
  if (CPU_EXT(PAE)) {
    int ptr = start_addr >> 30;
    start_addr -= ptr << 30;
    dirn = start_addr >> 21;
    start_addr -= dirn << 21;
    tabn = start_addr >> 12;
    start_addr -= tabn << 12;
  }
  else {
  #endif
  
  #ifdef WITH_PAE
  }
  #endif
  return 0;
}

#ifdef WITH_PAE
void paging_pae_enable() {
  asm volatile(
    "mov %%cr4, %%eax \n\t"
    "or  $0x20, %%eax \n\t"
    "mov %%eax, %%cr4 \n\t"
  : : : "eax");
}

uint64* paging_pae_create_directory_ptr() {
  uint64* dir_ptr = _pmalloc(4 * sizeof(uint64), 0x20, NULL);
  /*dir_ptr[0] = (uint32) paging_pae_create_directory();
  dir_ptr[1] = (uint32) paging_pae_create_directory();
  dir_ptr[2] = (uint32) paging_pae_create_directory();
  dir_ptr[3] = (uint32) paging_pae_create_directory();*/
  // unwinded.
  return dir_ptr;
}

uint64* paging_pae_create_directory(uint64 start_addr, uint32 bitmask, uint32 pt_bitmask) {
  uint64* dir = _pmalloc(512 * sizeof(uint64), 0x1000, NULL);
  int i;
  if (start_addr != ((uint64) - 1)) {
    for (i = 0; i < 512; i++) {
      dir[i] = (uint32) paging_pae_create_page_table(start_addr, pt_bitmask) | bitmask;
      start_addr += 512 * 4096;
    }
  } 
  return dir;
}

uint64* paging_pae_create_page_table(uint64 start_addr, uint32 bitmask) {
  uint64* page_tab = _pmalloc(512 * sizeof(uint64), 0x1000, NULL);
  int i;
  for (i = 0; i < 512; i++) {
    page_tab[i] = start_addr | bitmask;
    start_addr += 4096;
  }
  return page_tab;
}

#endif