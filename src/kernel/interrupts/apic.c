#include "apic.h"
#include "hardware/cpu/cpu.h"
#include "types.h"
#include "io.h"
#include "kernel/mp/mp.h"

extern uint32 cpu_ext_mask;

void apic_init() {
  if (CPU_EXT(APIC)) {
    mp_fp_t* fp = mp_get_fp();
    if (fp != NULL) {
      if (fp->mp_features[1] & MP_FP_FEAT1_IMCRP) {
        /*
         * IMCR register is used to configure APIC mode, which can work in 3
         * modes:
         *  * PIC mode,
         *  * Virtual Wire Mode,
         *  * Symmetric I/O Mode.
         * First 2 emulate functionality of PIC. Working on multiple processors
         * is possible only with 3rd mode.
         *
         * Presence of IMCR is not needed on systems, which do not explicitily
         * support first two modes.
         */
        outb()
      }
    }
  }
}
