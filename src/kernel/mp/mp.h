/* 
 * File:   mp.h
 * Author: Maciej
 *
 * Created on 16 sierpień 2010, 20:35
 */

/*!
 * \file mp.h
 * \author Maciej Małecki
 * 
 * \brief MultiProcessor functions
 *
 * mp.h defines functions used by kernel to perform operations on multiple
 * processor or just MultiProcessor-compliant systems
 */

#ifndef _MP_H
  #define	_MP_H

  #ifdef	__cplusplus
extern "C" {
  #endif

#include "types.h"

#define MP_FP_FEAT1_IMCRP (1 << 7)

/*!
 * \struct mp_fp
 * \brief  mp_fp contains basic information about MultiProcessor system
 * 
 * mp_fp struct contains basic information about MultiProcessor system,
 * including version of specification, it's features and address of
 * configuration table.
 *
 * \sa mp_get_fp()
 */
typedef struct mp_fp {
  char   signature[4];     /*!< signature, should always be "_MP_" */
  uint32 physical_address; /*!< Physical address of configuration table.
                                Please note, that when paging is enabled, you
                                have to convert it to virtual address! */
  uint8  length;           /*!< (length of mp_floating_pointer in bytes)/16 */
  uint8  mp_version;       /*!< MultiProcessor specification version */
  uint8  checksum;         /*!< struct checksum, all struct fields must sum to 0 */
  uint8  mp_features[5];   /*!< table of features:

                               * 0   - MP System Configuration Type
                               * 1   - bit 7 indicates presence of IMCR register
                               * 2-4 - reserved */
} __attribute__((packed)) mp_fp_t;
/*!
 * \struct mp_conf_table
 * \brief  mp_conf_table contains configuration of MultiProcessor system
 *
 * mp_floating_pointer struct contains information about configuration of
 * MultiProcessor system, including it's producer and address of local APIC.
 *
 * \sa multiproc_get_conf_table()
 */
typedef struct mp_conf_table {
  char   signature[4];      /*!< signature, should always be "PCMP"   */
  uint16 base_table_length; /*!< length of base configuration table   */
  uint8  mp_version,        /*!< MultiProcessor specification version */
         checksum;          /*!< checksum of base conf. table, all base field
                                 must sum to 0                        */
  char   oem_id[8];         /*!< manufacturer ID                      */
  char   product_id[12];    /*!< product family ID                    */
  uint32 oem_table;         /*!< physical address of OEM table        */
  uint16 oem_table_size,
         entry_count;       /*!< count of entries in base configuration table */
  uint32 local_apic;        /*!< base address of local APIC           */
  uint16 extended_table_length;
  uint8  extended_table_checksum,
         reserved;          /*!< "Get out of here, it's my place!" - said The
                                  Great Imperor Intel.

                                 "Yes, my master" - answered programmers, and
                                 from that day nobody used reserved bytes. */
} __attribute__((packed)) mp_conf_table_t;

/*!
 * \struct mp_processor_entry
 * \brief  Contains information about each processor in multiprocessor system
 *
 * mp_processor_entry contains information about each processor in multiproc
 * system, which we wouldn't be able to get using CPUID or classic
 * (non-MultiProcessor-compliant methods).
 */
typedef struct mp_processor_entry {
  uint8  entry_type,         /*!< specifies entry type in config. table, in
                                 mp_processor_entry should always be 0 */
         local_apic_id,
         local_apic_version, /*!< version of local APIC */
         cpu_flags;          /*!< describes each CPU (if is it usable and if is
                                 it a bootstrap processor) */
  uint32 cpu_signature,      /*!< contains information about each CPU (stepping,
                                  model, family) */
         features,           /*!< describes CPU features */
         reserved[2];        /*!< is there a real need to describe these? */
} __attribute__((packed)) mp_processor_entry_t;

/*!
 * \struct mp_bus_entry
 * \brief  Contains information about each bus installed in system.
 *
 * mp_bus_entry contains information about each bus in system, describing
 * it's type and ID.
 */
typedef struct mp_bus_entry {
  uint8 entry_type,  /*!< specifies entry type in config. table, in mp_bus_entry
                          should always be 1                    */
        bus_id;      /*!< unique bus id                         */
  char  bus_type[6]; /*!< bus type in string form, blank-filled */
} __attribute__((packed)) mp_bus_entry_t;

typedef struct mp_io_apic_entry {
  uint8  entry_type,      /*!< specifies entry type in config. table, in
                               mp_io_apic_entry should always be 1      */
         io_apic_id,      /*!< unique identified of each I/O APIC       */
         io_apic_version; /*!< I/O APIC version, probably won't be used */
  uint32 base_address;
} __attribute__((packed)) mp_io_apic_entry_t;

mp_fp_t*              mp_get_fp();
mp_conf_table_t*      mp_get_conf_table();
mp_processor_entry_t* mp_get_processors();
uint                  mp_get_processor_count();
mp_bus_entry_t*       mp_get_buses();
uint                  mp_get_bus_count();
mp_io_apic_entry_t*   mp_get_io_apics();
uint                  mp_get_io_apics_count();
void                  mp_init();

  #ifdef	__cplusplus
}
  #endif

#endif	/* _MP_H */

