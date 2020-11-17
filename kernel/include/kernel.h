#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>

#define GDT_CODE_SEL 1
#define GDT_DATA_SEL 2

#define KERNEL_CS (GDT_CODE_SEL * 8)
#define KERNEL_DS (GDT_DATA_SEL * 8)

struct os_registry
{
    // Information related to the CPUs.
    unsigned cpu_count;
    struct cpu_info
    {
        unsigned cpu_id;
        unsigned local_apic_id;
        unsigned local_apic_flags;
    } cpu_info[32];

    // Local APIC.
    uintptr_t local_apic_addr;
    
    // I/O APIC.
    uintptr_t io_apic_addr;
};

extern struct os_registry g_registry;

#endif /* KERNEL_H_ */