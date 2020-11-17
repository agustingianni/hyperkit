#include "io.h"
#include "bmv.h"
#include "asm.h"
#include "pic.h"
#include "utils.h"
#include "string.h"
#include "kernel.h"
#include "interrupts.h"

extern void interrupt_dispatcher();

#define SDT_SYS386IGT 0x0e
#define SDT_SYS386TGT 0x0f

static struct idt_entry g_idt[IDT_ENTRIES];

static void idt_load(struct idt_entry *idt, size_t size)
{
    lidt(idt, size - 1);
}

static void idt_set_entry(struct idt_entry *entry, void *base, int sel, int type, int dpl)
{
    entry->ie_lobase = LOWORD(base);
    entry->ie_hibase = HIWORD(base);
    entry->ie_sel = sel;
    entry->ie_type = type;
    entry->ie_dpl = dpl;
    entry->ie_zero = 0;
    entry->ie_present = 1;
}

// Does not clear the interrupt flag.
static void idt_set_trap_gate(struct idt_entry *idt, int n, void *handler, int dpl)
{
    idt_set_entry(&idt[n], handler, KERNEL_CS, SDT_SYS386TGT, dpl);
}

// Clears the interrupt flag.
static void idt_set_interrupt_gate(struct idt_entry *idt, int n, void *handler, int dpl)
{
    idt_set_entry(&idt[n], handler, KERNEL_CS, SDT_SYS386IGT, dpl);
}

static void init_idt()
{
    // Handlers defined in start.s.
    extern void *interrupt_handler_table[IDT_ENTRIES];

    bmv_printf("Initializing %u IDT entries.\n", IDT_ENTRIES);

    // Register processor exception handlers.
    for (int i = 0; i < IDT_ENTRIES; i++)
    {
        idt_set_interrupt_gate(g_idt, i, interrupt_handler_table[i], 0);
    }

    // Load the interrupt table.
    idt_load(g_idt, IDT_SIZE);
}

static void init_apic()
{
    // One Local APIC for each CPU

    // One I/O APIC for each peripheral bus.
}

struct interrupt_context
{
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t eax;
    uint16_t ds;
    uint16_t __dsh;
    uint16_t es;
    uint16_t __esh;
    uint16_t fs;
    uint16_t __fsh;
    uint16_t gs;
    uint16_t __gsh;

    uint32_t exception_number;
    uint32_t error_code;

    uint32_t ip;
    uint16_t cs;
    uint16_t __csh;
    uint32_t flags;
    uint32_t sp;
    uint16_t ss;
    uint16_t __ssh;
};

union cpu_flags
{
    struct
    {
        unsigned cf : 1;
        unsigned R1 : 1;
        unsigned pf : 1;
        unsigned R3 : 1;
        unsigned af : 1;
        unsigned R5 : 1;
        unsigned zf : 1;
        unsigned sf : 1;
        unsigned tf : 1;
        unsigned if_ : 1;
        unsigned df : 1;
        unsigned of : 1;
        unsigned iopl : 2;
        unsigned nt : 1;
        unsigned R15 : 1;
        unsigned rf : 1;
        unsigned vm : 1;
        unsigned ac : 1;
        unsigned vif : 1;
        unsigned vip : 1;
        unsigned id : 1;
        unsigned R31_22 : 10;
    } __attribute__((packed));

    uint32_t value;
};

static volatile unsigned long g_ticks = 0;

void interrupt_handler(struct interrupt_context *context)
{
    // union cpu_flags flags;
    // flags.value = context->flags;
    // bmv_printf("  flags=0x%lx\n", flags.value);
    // bmv_printf("  flags.cf=%d\n", flags.cf);
    // bmv_printf("  flags.pf=%d\n", flags.pf);
    // bmv_printf("  flags.af=%d\n", flags.af);
    // bmv_printf("  flags.zf=%d\n", flags.zf);
    // bmv_printf("  flags.sf=%d\n", flags.sf);
    // bmv_printf("  flags.tf=%d\n", flags.tf);
    // bmv_printf("  flags.if=%d\n", flags.if_);
    // bmv_printf("  flags.df=%d\n", flags.df);
    // bmv_printf("  flags.of=%d\n", flags.of);
    // bmv_printf("  flags.iopl=%d\n", flags.iopl);
    // bmv_printf("  flags.nt=%d\n", flags.nt);
    // bmv_printf("  flags.rf=%d\n", flags.rf);
    // bmv_printf("  flags.vm=%d\n", flags.vm);
    // bmv_printf("  flags.ac=%d\n", flags.ac);
    // bmv_printf("  flags.vif=%d\n", flags.vif);
    // bmv_printf("  flags.vip=%d\n", flags.vip);
    // bmv_printf("  flags.id=%d\n", flags.id);

    switch (context->exception_number)
    {
    case 32:
        __sync_fetch_and_add(&g_ticks, 1);
        if (!(g_ticks % 1000))
            bmv_printf("Tick\n");

        bmv_printf("g_ticks=%lu\n", g_ticks);

        pic_send_eoi(0);

        break;

    default:
        bmv_printf("Handling interrupt.\n");
        bmv_printf("  exception_number=%lu\n", context->exception_number);
        bmv_printf("  error_code=%lu\n", context->error_code);
        // bmv_printf("  edi=0x%lx\n", context->edi);
        // bmv_printf("  esi=0x%lx\n", context->esi);
        // bmv_printf("  ebp=0x%lx\n", context->ebp);
        // bmv_printf("  edx=0x%lx\n", context->edx);
        // bmv_printf("  ecx=0x%lx\n", context->ecx);
        // bmv_printf("  ebx=0x%lx\n", context->ebx);
        // bmv_printf("  eax=0x%lx\n", context->eax);
        // bmv_printf("  ds=%d\n", context->ds);
        // bmv_printf("  es=%d\n", context->es);
        // bmv_printf("  fs=%d\n", context->fs);
        // bmv_printf("  gs=%d\n", context->gs);
        bmv_printf("  ip=0x%lx\n", context->ip);
        // bmv_printf("  cs=%d\n", context->cs);
        bmv_printf("  flags=0x%lx\n", context->flags);
        // bmv_printf("  sp=0x%lx\n", context->sp);
        // bmv_printf("  ss=%d\n", context->ss);
        // __asm__ __volatile__("hlt");
        break;
    }
}

void init_interrupts()
{
    g_ticks = 0;
    irq_disable();
    {
        init_idt();

        init_pic();

        init_apic();
    }
    irq_enable();
}
