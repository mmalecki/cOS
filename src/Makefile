#                       REALLY IMPORTANT NOTE!
#                                             for Windows users
#
# No way you can compile kernel under Windows using VC++. Just no. Don't even try.
#
# Please also note, that even if you're using MinGW, you HAVE TO cross-compile
# (so the output format is ELF).
#
# In fact, I use sometimes Windows as my working machine, but build is then
# performed on Linux machine, using built-in (?) NetBeans feature that allows
# you to choose developement host from network (it will work even over
# Internet, but that would be a bit uncomfortable, since it compiles source code,
# but does not copy build objects back to working machine).
# All you need to perform remote build is working net connection and SSH server
# on build machine.
#
# There are no problems under any Linux with even most basic build tools.

BIN         = ../bin/cOS
INCS        = -I"libc" -I"common" -I"."
DEFINES     = -DWITH_SSE -DWITH_SSE2 -DWITH_APIC #-DWITH_PAE
CFLAGS      = $(INCS) $(DEFINES) -Wall -Wextra -nostdlib -nostartfiles \
              -nodefaultlibs -ffreestanding -m32 -O2
OBJP        = ../obj/
LDFLAGS     = -m elf_i386 -Tlinker.ld
NASMFLAGS   = -f elf
CC          = gcc
LD          = ld
RM          = rm -f
NASM        = nasm
GAS         = $(CC)
GASFLAGS    = $(CFLAGS)
CSOURCES    = boot/kcall.c common/io.c hardware/bios/cmos.c kernel/mp/mp.c \
              hardware/bios/rtc.c hardware/cpu/cpu.c hardware/graphics/monitor.c \
              kernel/kernel.c kernel/gdt/gdt.c kernel/interrupts/idt.c kernel/screen.c \
              kernel/interrupts/int.c kernel/interrupts/irq.c kernel/mem/paging.c \
              kernel/mem/pmalloc.c kernel/res_mgr/res_mgr.c libc/math.c libc/stdio.c \
              libc/stdlib.c libc/string.c hardware/bios/bda.c kernel/panic.c
NASMSOURCES = boot/boot_hhk.s kernel/gdt/gdt_asm.s kernel/interrupts/idt_asm.s \
              kernel/interrupts/isr.s
GASSOURCES  = #boot/boot.S
COBJ        = $(CSOURCES:%.c=%.o)
NASMOBJ     = $(NASMSOURCES:%.s=%.o)
GASOBJ      = $(GASSOURCES:%.S=%.o)
OBJ         = $(GASOBJ) $(NASMOBJ) $(COBJ)
LINKOBJ     = $(patsubst %.o,$(OBJP)%.o,$(notdir $(OBJ)))

all: $(BIN)

$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $(BIN) $(LINKOBJ)

$(COBJ): %.o: %.c
	$(CC) $(CFLAGS) -o $(OBJP)$(@F) -c $<

$(NASMOBJ): %.o: %.s
	$(NASM) $(NASMFLAGS) -o $(OBJP)$(@F) $<

$(GASOBJ): %.o: %.S
	$(GAS) $(GASFLAGS) -o $(OBJP)$(@F) -c $<

%.s:
	
clean:
	$(RM) $(BIN) $(OBJP)*.o

.PHONY: all clean
