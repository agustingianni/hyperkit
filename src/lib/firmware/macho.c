#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xhyve/vmm/vmm_api.h>
#include <xhyve/firmware/macho.h>
#include <xhyve/support/segments.h>
#include <xhyve/support/specialreg.h>

static struct
{
    char *kernel;
} config;

// OS memory layout
//  0000_0000h: Zero page
//  0000_1000h: GDT
//  0000_2000h: Stack.
//  0010_0000h: Kernel
#define GUEST_BASE_ZEROPAGE 0x00000000
#define GUEST_BASE_GDT 0x00001000
#define GUEST_BASE_STACK_BASE 0x00002000
#define GUEST_BASE_STACK_TOP 0x00003000
#define GUEST_BASE_KERNEL 0x00100000

#define GUEST_SELECTOR_SIZE 8

// NOTE: Keep this updated if we add any more selectors.
#define GUEST_NULL_SEL 0
#define GUEST_CODE_SEL 1
#define GUEST_DATA_SEL 2
#define GUEST_LAST_SEL 3
#define GUEST_GDTR_LIMIT (GUEST_LAST_SEL * GUEST_SELECTOR_SIZE - 1)

#define GUEST_NULL_SEL_OFFSET (0 * GUEST_SELECTOR_SIZE)
#define GUEST_CODE_SEL_OFFSET (1 * GUEST_SELECTOR_SIZE)
#define GUEST_DATA_SEL_OFFSET (2 * GUEST_SELECTOR_SIZE)

#define DESC_PRESENT 0x00000080
#define DESC_DEF32 0x00004000
#define DESC_GRAN 0x00008000
#define DESC_UNUSABLE 0x00010000

int macho_init(char *kernel_path, char *module_list, char *kernel_append)
{
    printf("kernel_path=%s | module_list=%s | kernel_append=%s\n",
           kernel_path, module_list, kernel_append);

    config.kernel = kernel_path;

    return 0;
}

uint64_t
macho(void)
{
    // 1. Map the guest physical address range [0, lowmem_size).
    char *guest_memory = xh_vm_map_gpa(0, xh_vm_get_lowmem_size());
    void *zero_page = (void *)&guest_memory[GUEST_BASE_ZEROPAGE];
    uint64_t *gdt_table = (uint64_t *)&guest_memory[GUEST_BASE_GDT];
    void *kernel_base = &guest_memory[GUEST_BASE_KERNEL];

    printf("Mapped guest memory to %p\n", (void *)guest_memory);
    printf("Mapped zero page to    %p\n", (void *)zero_page);
    printf("Mapped GDT table to    %p\n", (void *)gdt_table);
    printf("Mapped kernel base to  %p\n", (void *)kernel_base);

    // 2. Prepare the zero page.
    memset(zero_page, 0, 0x1000);

    // 3. Create a global descriptor table.
    gdt_table[GUEST_NULL_SEL] = 0x0000000000000000;
    gdt_table[GUEST_CODE_SEL] = 0x00cf9a000000ffff;
    gdt_table[GUEST_DATA_SEL] = 0x00cf92000000ffff;

    // 4. Put the CPU into a well defined initial state.
    xh_vcpu_reset(0);

    // 5. Set the value of the GDT register.
    xh_vm_set_desc(0, VM_REG_GUEST_GDTR, GUEST_BASE_GDT, GUEST_GDTR_LIMIT, 0);

    // 6. Set the value of the guests selectors in the VMCS.
    xh_vm_set_desc(0, VM_REG_GUEST_CS, 0, 0xffffffff,
                   DESC_GRAN | DESC_DEF32 | DESC_PRESENT | SDT_MEMERA);

    xh_vm_set_desc(0, VM_REG_GUEST_DS, 0, 0xffffffff,
                   DESC_GRAN | DESC_DEF32 | DESC_PRESENT | SDT_MEMRWA);

    xh_vm_set_desc(0, VM_REG_GUEST_ES, 0, 0xffffffff,
                   DESC_GRAN | DESC_DEF32 | DESC_PRESENT | SDT_MEMRWA);

    xh_vm_set_desc(0, VM_REG_GUEST_SS, 0, 0xffffffff,
                   DESC_GRAN | DESC_DEF32 | DESC_PRESENT | SDT_MEMRWA);

    // 7. Point the segment selector registers to their segments.
    xh_vm_set_register(0, VM_REG_GUEST_CS, GUEST_CODE_SEL_OFFSET);
    xh_vm_set_register(0, VM_REG_GUEST_DS, GUEST_DATA_SEL_OFFSET);
    xh_vm_set_register(0, VM_REG_GUEST_ES, GUEST_DATA_SEL_OFFSET);
    xh_vm_set_register(0, VM_REG_GUEST_SS, GUEST_DATA_SEL_OFFSET);

    // 8. Enable numeric error reporting and protected mode.
    xh_vm_set_register(0, VM_REG_GUEST_CR0, CR0_NE | CR0_PE);

    // 9. Set the address of RIP to the start of our code.
    xh_vm_set_register(0, VM_REG_GUEST_RIP, GUEST_BASE_KERNEL);

    // 10. Set the address of RSP to our stack.
    xh_vm_set_register(0, VM_REG_GUEST_RSP, GUEST_BASE_STACK_TOP);

    // 11. Read the kernel image into guest memory.
    FILE *fd;
    if (!(fd = fopen(config.kernel, "r")))
    {
        perror("Could not open kernel image.");
        return 0;
    }

    fseek(fd, 0L, SEEK_END);
    size_t kernel_size = (size_t)ftell(fd);

    fseek(fd, 0, SEEK_SET);
    if (!fread(kernel_base, kernel_size, 1, fd))
    {
        perror("Could not read kernel image.");
        fclose(fd);
        return 0;
    }

    fclose(fd);

    return GUEST_BASE_KERNEL;
}
