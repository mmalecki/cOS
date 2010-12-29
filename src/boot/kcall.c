/* kernel.c - the C part of the kernel 
   
  Copyright (C) 1999  Free Software Foundation, Inc.
     
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
     
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
     
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */
     
#include "multiboot.h"
#include "kernel/kernel.h"
#include "kernel/screen.h"
#include "macros.h"

multiboot_info_t* multiboot_info; // so that kernel could access it anytime

/* Forward declarations. */
void kcall(unsigned long magic, unsigned long addr);
static void itoa (char *buf, int base, int d);
void printf (const char *format, ...);
     
/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */
void kcall(unsigned long magic, unsigned long addr) {
  screen_init();

  /* Am I booted by a Multiboot-compliant boot loader? */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
    return;
  }
     
  /* Set MBI to the address of the Multiboot information structure. */
  multiboot_info = (multiboot_info_t *) addr;

  // physical -> virtual conversion
  multiboot_info->cmdline          += 0xC0000000;
  multiboot_info->mmap_addr        += 0xC0000000;
  multiboot_info->mods_addr        += 0xC0000000;
  multiboot_info->boot_loader_name += 0xC0000000;
     
  /* Print out the flags. */
  printf ("flags = 0x%x\n", (unsigned) multiboot_info->flags);
     
  /* Are mem_* valid? */
  if (IS_BIT_SET(multiboot_info->flags, 0)) {
    printf ("mem_lower = %uKB, mem_upper = %uKB\n",
       (unsigned) multiboot_info->mem_lower, (unsigned) multiboot_info->mem_upper);
  }
     
  /* Is boot_device valid? */
  if (IS_BIT_SET(multiboot_info->flags, 1))
    printf ("boot_device = 0x%x\n", (unsigned) multiboot_info->boot_device);
     
  /* Is the command line passed? */
  if (IS_BIT_SET(multiboot_info->flags, 2))
    printf ("cmdline = %s\n", (char *) multiboot_info->cmdline);
  
  /* Are mods_* valid? */
  if (IS_BIT_SET(multiboot_info->flags, 3)) {
    multiboot_module_t *mod;
    unsigned int i;
     
    printf("mods_count = %d, mods_addr = 0x%x\n", (int) multiboot_info->mods_count,
       (int) multiboot_info->mods_addr);
    for (i = 0, mod = (multiboot_module_t *) multiboot_info->mods_addr;
       i < multiboot_info->mods_count; i++, mod++) {
      printf(
        " mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
        (unsigned) mod->mod_start,
        (unsigned) mod->mod_end,
        (char *) mod->cmdline);
    }
  }
     
  /* Bits 4 and 5 are mutually exclusive! */
  if (IS_BIT_SET(multiboot_info->flags, 4) && IS_BIT_SET(multiboot_info->flags, 5)) {
    screen_puts("Both bits 4 and 5 are set.\n");
    return;
  }
     
  /* Is the symbol table of a.out valid? */
  if (IS_BIT_SET(multiboot_info->flags, 4)) {
    multiboot_aout_symbol_table_t *multiboot_aout_sym = &(multiboot_info->u.aout_sym);
     
    printf(
      "multiboot_aout_symbol_table: tabsize = 0x%0x, "
      "strsize = 0x%x, addr = 0x%x\n",
      (unsigned) multiboot_aout_sym->tabsize,
      (unsigned) multiboot_aout_sym->strsize,
      (unsigned) multiboot_aout_sym->addr);
  }
     
  /* Is the section header table of ELF valid? */
  if (IS_BIT_SET(multiboot_info->flags, 5)) {
    multiboot_elf_section_header_table_t *multiboot_elf_sec = 
       &(multiboot_info->u.elf_sec);
     
    printf (
      "multiboot_elf_sec: num = %u, size = 0x%x,"
      " addr = 0x%x, shndx = 0x%x\n",
      (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
      (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
  }
     
  /* Are mmap_* valid? */
  if (IS_BIT_SET(multiboot_info->flags, 6)) {
    multiboot_memory_map_t *mmap;
     
    printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n", 
       (unsigned) multiboot_info->mmap_addr, (unsigned) multiboot_info->mmap_length);
    
    for (mmap = (multiboot_memory_map_t *) multiboot_info->mmap_addr;
       (unsigned long) mmap < multiboot_info->mmap_addr + multiboot_info->mmap_length;
          mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
             + mmap->size + sizeof (mmap->size))) {
             printf (" size = 0x%x, base_addr = 0x%x%x,"
                     " length = 0x%x%x, type = 0x%x\n",
                     (unsigned) mmap->size,
                     mmap->addr >> 32,
                     mmap->addr & 0xffffffff,
                     mmap->len >> 32,
                     mmap->len & 0xffffffff,
                     (unsigned) mmap->type);
    }
  }
  
  kmain(multiboot_info);
}

     
/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to 'd', interpret that D is decimal, and if BASE is
   equal to 'x', interpret that D is hexadecimal. */
     
static void itoa(char *buf, int base, int d) {
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
     
  /* If %d is specified and D is minus, put `-' in the head. */
  if (base == 'd' && d < 0) {
    *p++ = '-';
    buf++;
    ud = -d;
  }
  else if (base == 'x')
    divisor = 16;
     
  /* Divide UD by DIVISOR until UD == 0. */
  do {
    int remainder = ud % divisor;   
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
  }
  while (ud /= divisor);
     
  /* Terminate BUF. */
  *p = 0;
     
  /* Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }
}
     
/* Format a string and print it on the screen, just like the libc
   function printf. */
void printf (const char *format, ...) {
  char **arg = (char **) &format;
  int c;
  char buf[20];
     
  arg++;
     
  while ((c = *format++) != 0) {
    if (c != '%')
      screen_putc(c);
    else {
      char *p;
      c = *format++;
      
      switch (c) {
        case 'd':
        case 'u':
        case 'x':
          itoa (buf, c, *((int *) arg++));
          p = buf;
          goto string;
          break;

        case 's':
          p = *arg++;
          if (!p)
            p = "(null)";

          string:
            while (*p)
              screen_putc(*p++);
            break;
     
        default:
          screen_putc(*((int *) arg++));
          break;
      }
    }
  }
}

