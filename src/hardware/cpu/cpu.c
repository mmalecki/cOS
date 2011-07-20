#include "cpu.h"
#include "types.h"
#include "macros.h"
#include "kernel/screen.h"
#include "kernel/mem/pmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cpu_info_t* cpu_info     = NULL;
uint32      cpu_ext_mask = 0;

#define CPU_FEAT_INFO(feat)       \
  kputs("    * "#feat);           \
  if (CPU_EXT(feat))              \
    kputs(" (supported by CPU)"); \
  kputc('\n');                    

char* cpuid_make_vendor_str(uint32 ebx, uint32 ecx, uint32 edx);

int cpu_init(int partial_init) {
  if (partial_init == 0)
    partial_init = CPU_INIT_ALL;
  
  int result = 0;

  if (IS_FLAG_SET(partial_init, CPU_INIT_FPU))
    result |= (cpu_fpu_initialize() * CPU_INIT_FPU);
  /*
   * Note about this construction:
   * since TRUE is defined as 1 (in types.h), FALSE as 0, and CPU_INIT_FPU has
   * only one bit set, the same bit would be set, depending if function will
   * return TRUE or FALSE.
   */
  if (IS_FLAG_SET(partial_init, CPU_INIT_CPUID)) {
    cpu_get_info();

    char buf[512];
    sprintf(buf,
      "\n   CPU information dump:\n"
      "    Vendor:   %s\n"
      "    Family:   %i\n"
      "    Model:    %i\n"
      "    Stepping: %i\n"
      "    Type:     %i\n",
      cpu_info->vendor, cpu_info->family, cpu_info->model, cpu_info->stepping,
      cpu_info->type);
    kputs(buf);
    kputs("   This build supports CPU features:\n");
    #ifdef __SSE__
      CPU_FEAT_INFO(SSE)
    #endif
    #ifdef __SSE2__
      CPU_FEAT_INFO(SSE2)
    #endif
    #ifdef WITH_APIC 
      CPU_FEAT_INFO(APIC)
    #endif
    #ifdef WITH_PAE  
      CPU_FEAT_INFO(PAE)
    #endif

    result |= TRUE * CPU_INIT_CPUID;
  }
  return result ^ partial_init;
}

bool cpu_fpu_initialize() {
  return TRUE;
}

cpu_info_t* cpu_get_info() {
  if (cpu_info == NULL) {
    cpu_info = (cpu_info_t*) pmalloc(sizeof(cpu_info_t));

    uint32 eax, ebx, ecx, edx;
    cpuid(0, &eax, &ebx, &ecx, &edx);
    cpu_info->vendor   = cpuid_make_vendor_str(ebx, ecx, edx);
    cpu_info->max_call = eax;

    cpuid(1, &eax, &ebx, &cpu_info->ecx_features, &cpu_info->edx_features);
    cpu_info->stepping = CPUID_STEPPING(eax);
    cpu_info->model    = CPUID_MODEL(eax);
    cpu_info->family   = CPUID_FAMILY(eax);
    cpu_info->type     = CPUID_TYPE(eax);

    cpu_info->ext = (CPU_SUPPORTS(cpu_info->edx_features,  EDX_SSE)) ?
       CPU_EXT_SSE  : 0;
    cpu_info->ext |= (CPU_SUPPORTS(cpu_info->edx_features, EDX_SSE2)) ?
       CPU_EXT_SSE2 : 0;
    cpu_info->ext |= (CPU_SUPPORTS(cpu_info->edx_features, EDX_APIC)) ?
       CPU_EXT_APIC : 0;
    cpu_info->ext |= (CPU_SUPPORTS(cpu_info->edx_features, EDX_PAE)) ?
       CPU_EXT_PAE : 0;

    cpu_ext_mask = cpu_info->ext;
  }
  return cpu_info;
}

void cpuid(uint32 request, uint32* eax, uint32* ebx, uint32* ecx, uint32* edx) {
  asm volatile("cpuid" : "=a" (*eax),"=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (request));
}

char* cpuid_make_vendor_str(uint32 ebx, uint32 ecx, uint32 edx) {
  char* str = (char*) pmalloc(13);
  str[0] = ebx;
  str[1] = (ebx >> 8) & 0xFF;
  str[2] = (ebx >> 16) & 0xFF;
  str[3] = (ebx >> 24) & 0xFF;
  str[4] = edx & 0xFF;
  str[5] = (edx >> 8) & 0xFF;
  str[6] = (edx >> 16) & 0xFF;
  str[7] = (edx >> 24) & 0xFF;
  str[8] = ecx & 0xFF;
  str[9] = (ecx >> 8) & 0xFF;
  str[10] = (ecx >> 16) & 0xFF;
  str[11] = (ecx >> 24) & 0xFF;
  str[12] = '\0';
  return str;
}