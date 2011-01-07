ENTRY(_loader)
OUTPUT_FORMAT(elf32-i386)

KERNEL_VIRTUAL_BASE = 0xC0000000;

SECTIONS {
  . = KERNEL_VIRTUAL_BASE + 0x100000;

  .text : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE) {
    *(.text)
    *(.rodata*)
  }

  .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE) {
    *(.data)
  }

  .bss : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE) {
    _sbss = .;
    *(COMMON)
    *(.bss)
    _ebss = .;
  }

  end = .; _end = .; __end = .;
}