/*
 * kernel.c - main kernel source
 * Not really big, isn't it?
 * And let it stay so. Keep it as small, as possible.
 */

#include "kernel.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "boot/multiboot.h"
#include "res_mgr/res_mgr.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "interrupts/irq.h"
#include "mem/paging.h"
#include "hardware/bios/cmos.h"
#include "hardware/bios/rtc.h"
#include "screen.h"
#include "hardware/cpu/cpu.h"
#include "defines.h"
#include "types.h"
#include "mp/mp.h"
#include "panic.h"

void khardware_initialize();
void kinitialize_modules();
void kself_tests();
void kself_test(char* name, bool result);

void kmain(multiboot_info_t* mbi) {
  kcls();
  kputs("\ncOS kernel\n"AUTHOR_NOTE"\n"COMPILE_NOTE"\n\nBoot data:\n Command line: ");
  kputs((char*) mbi->cmdline);
  kputs("\n Bootloader name: ");
  kputs((char*) mbi->boot_loader_name);
  kputs("\n\n");
  
  if (strstr((char*) mbi->cmdline, "--self-test") != NULL)
    kself_tests();
  else
    kputs("To run self-tests boot with --self-test switch.\n\n");
  
  khardware_initialize();
  kinitialize_modules();
}

void khardware_initialize() {
  screen_puts("Initializing basic hardware:\n");
  screen_puts(" Initializing CPU...");
  int cpu = cpu_init(CPU_INIT_ALL);
  if (cpu != 0) {
    char buf[64];
    sprintf(buf, " Failed with code 0x%x.\n", cpu);
    screen_putsa(buf, 0x0C);
  }
  else
    screen_putsa(" OK.\n", 0x0A);
  
  screen_puts("Basic hardware initialized.\n\n");
}

void kinitialize_modules() {
  screen_puts("Initialing built-in kernel modules:\n");
  screen_puts(" Initializing GDT...");
  gdt_init();
  screen_putsa(" OK.\n", 0x0A);

  screen_puts(" Initializing IDT...");
  idt_init();
  screen_putsa(" OK.\n", 0x0A);

  screen_puts(" Initializing PIC and IRQ...");
  irq_init();
  screen_putsa(" OK.\n", 0x0A);

  screen_puts(" Initializing paging...");
  paging_init();
  screen_putsa(" OK.\n", 0x0A);
  
  screen_puts(" Initializing MultiProcessor...");
  mp_init();
  screen_putsa(" OK.\n", 0x0A);

  screen_puts("Built-in kernel modules initialized.\n\n");
}

void kself_tests() {
  char* str;
  screen_puts("Running self-tests:\n");

  kself_test("pow_ii", (pow_ii(3, 3) == 27.0));
  kself_test("pow_di", (pow_di(2.5, 3) == 15.625));
  kself_test("strcmp (0)", (strcmp("string", "string")) == 0);
  kself_test("strcmp (1)", (strcmp("string", "strang")) > 0);
  kself_test("strcmp (2)", (strcmp("string", "strzng")) < 0);
  kself_test("strncmp (0)", (strncmp("string", "strzng", 3)) == 0);
  kself_test("strncmp (1)", (strncmp("string", "string", 6)) == 0);
  kself_test("strncmp (2)", (strncmp("string", "string", 8)) == 0);
  kself_test("strncmp (3)", (strncmp("string", "strang", 4)) > 0);
  kself_test("strncmp (4)", (strncmp("string", "strzng", 4)) < 0);
  
  str = "string that came out of output is correct";
  kself_test("strchr", strncmp(strchr(str, 'o'), "out ", 4) == 0);
  kself_test("strstr", strncmp(strstr(str, "output"), "output", 6) == 0);

  screen_puts("Self-testing done.\n\n");
}

void kself_test(char* name, bool result) {
  screen_putc(' ');
  screen_putsa(name, (result) ? 0x0A : 0x0C);
  screen_putsa((result) ? " passed\n" : " failed\n", (result) ? 0x0A : 0x0C);
  if (!result) {
    char msg[4096];
    sprintf(msg,
      "%s self-test failed. This situation shouldn't take place unless you've "
      "changed source of kernel, which shouldn't take place even more than this "
      "situation. If you didn't change source code and you're sure that your kernel "
      "came from trusted source, contact customer support.\n"
      "Yet, if you did modificate kernel, revert your changes. There are better "
      "ways of improving system performance than crashing it.", name);
    kpanic(msg);
  }
}
