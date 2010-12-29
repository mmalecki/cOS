/*!
 * \file  cpu.h
 * \brief CPU functions
 *
 * cpu.h defines functions and structures used by kernel to perform basic CPU
 * operations (ie. CPUID, FPU initialization).
 */

#ifndef _CPU_H
#define _CPU_H

#include "types.h"

#define CPU_INIT_FPU   1
#define CPU_INIT_CPUID 2
#define CPU_INIT_ALL   CPU_INIT_FPU | CPU_INIT_CPUID

#define CPUID_GET_VENDOR   0
#define CPUID_GET_FEATURES 1

#define CPUID_STEPPING(eax) (eax & 0x0F)
#define CPUID_MODEL(eax)    ((eax >> 4) & 0x0F)
#define CPUID_FAMILY(eax)   ((eax >> 8) & 0x0F)
#define CPUID_TYPE(eax)     ((eax >> 12) & 0x0F)

#define CPUID_FEATURE_EDX_PAE  (1 << 6)
#define CPUID_FEATURE_EDX_APIC (1 << 9)
#define CPUID_FEATURE_EDX_SSE  (1 << 25)
#define CPUID_FEATURE_EDX_SSE2 (1 << 26)

#define CPU_SUPPORTS(bitmask, feature) (bitmask & CPUID_FEATURE_##feature)

#define CPU_EXT_SSE  (1)
#define CPU_EXT_SSE2 (1 << 1)
#define CPU_EXT_APIC (1 << 2)
#define CPU_EXT_PAE  (1 << 3)

#define CPU_EXT(ext) (cpu_ext_mask & CPU_EXT_##ext)

/*!
 * \struct cpu_info
 * \brief  cpu_info structure carries information about CPU
 *
 * cpu_info structure is used by different parts of kernel to get to know what
 * features has CPU got, who was it produced by, etc.
 *
 * \sa cpu_get_info(), http://wiki.osdev.org/CPUID
 */
typedef struct cpu_info {
  uint32 ecx_features, /*!< features information returned in ECX, may be bogus
                            on old CPUs                                     */
         edx_features, /*!< features information returned in EDX            */
         max_call;     /*!< holds information about maximum cpuid request # */
  uint8  stepping, model, family, type;
  uint32 ext;          /*!< bitmask representing processor extensions, like SSE,
                            which are especially interesting for kernel     */
  char*  vendor;       /*!< string of 13 (13th is \0) characters representing
                            CPU vendor                                      */
} cpu_info_t, *pcpu_info_t;

/*
 * In result of cpu_initialize() there are set only flags, which did fail.
 *
 * cpu_initialize(0) == cpu_initialize(CPU_INIT_ALL)
 */
int         cpu_init(int partial_init);
bool        cpu_fpu_initialize();
cpu_info_t* cpu_get_info();
void        cpuid(uint32 request, uint32* eax, uint32* ebx, uint32* ecx, uint32* edx);

#endif