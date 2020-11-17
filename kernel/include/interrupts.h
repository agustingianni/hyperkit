#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "asm.h"

#define IDT_ENTRIES 256
#define IDT_SIZE (sizeof(struct idt_entry) * IDT_ENTRIES)

struct idt_entry
{
    unsigned ie_lobase : 16;
    unsigned ie_sel : 16;
    unsigned ie_zero : 8;
    unsigned ie_type : 5;
    unsigned ie_dpl : 2;
    unsigned ie_present : 1;
    unsigned ie_hibase : 16;
} __attribute__((packed));

static inline void irq_disable(void)
{
    cli();
}

static inline void irq_enable(void)
{
    sti();
}

void init_interrupts();

#endif /*INTERRUPTS_H_*/