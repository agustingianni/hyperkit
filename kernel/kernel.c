#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cpu.h"
#include "bmv.h"
#include "acpi.h"
#include "timer.h"
#include "kernel.h"
#include "string.h"
#include "processors.h"
#include "interrupts.h"

struct os_registry g_registry;

// IMPORTANT NOTES
//
// Paging
//  When we are working on 32 bit mode we only use segmentation in a linear fashion.
//  If we ever support 64 bits, we should create a 1:1 page table mapping.
//
// About TSS
//  We are working under the assumption that since we are only using ring0
//  we don't need a TSS. If we ever implement another privilege level, we should
//  create a TSS for each CPU.
//
// Stacks
//  We need one stack for each CPU. So far we only have one.
#include "io.h"

int kernel_main(void)
{
    // http://phrack.org/papers/escaping_from_freebsd_bhyve.html
    irq_disable();

    memset(&g_registry, 0, sizeof(g_registry));

    init_acpi();

    init_cpu();

    init_interrupts();

    init_timer();

    init_processors();

    bmv_println("Kernel initialized!");

    // Yield.
    while (1)
    {
        bmv_println("Loop");
        __asm__ __volatile__("sti; nop; int $32; cli");
    }

    return 0;
}
