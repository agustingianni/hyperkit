#include <stddef.h>

#include "bmv.h"
#include "acpi.h"
#include "string.h"
#include "printf.h"
#include "kernel.h"

static void *acpi_find_base()
{
    // Search for ACPI_SIG_RSDP in the first megabyte of memory.
    void *memory = 0;
    size_t size = 0x100000;
    return memmem(memory, size, ACPI_SIG_RSDP, 8);
}

static void *acpi_find_sdt(const ACPI_TABLE_RSDT *root, const char *name)
{
    unsigned entries = (root->Header.Length - sizeof(ACPI_TABLE_HEADER)) / sizeof(UINT32);
    for (unsigned i = 0; i < entries; i++)
    {
        ACPI_TABLE_HEADER *header = (ACPI_TABLE_HEADER *)root->TableOffsetEntry[i];
        if (!strncmp(header->Signature, name, 4))
            return header;
    }

    return NULL;
}

void init_acpi()
{
    // https://wiki.osdev.org/RSDP
    ACPI_TABLE_RSDP *rsdp_table = (ACPI_TABLE_RSDP *)acpi_find_base();

    // https://wiki.osdev.org/RSDT
    ACPI_TABLE_RSDT *rsdt_table = (ACPI_TABLE_RSDT *)rsdp_table->RsdtPhysicalAddress;

    // Find "APIC": Multiple APIC Description Table (MADT)
    ACPI_TABLE_MADT *madt_table = (ACPI_TABLE_MADT *)acpi_find_sdt(rsdt_table, "APIC");
    g_registry.local_apic_addr = madt_table->Address;

    // Go to the subtables and parse them.
    uintptr_t offset = sizeof(*madt_table);
    while (offset < madt_table->Header.Length)
    {
        ACPI_SUBTABLE_HEADER *madt_subtable = (ACPI_SUBTABLE_HEADER *)((uintptr_t)madt_table + offset);
        offset += madt_subtable->Length;

        switch (madt_subtable->Type)
        {
        case ACPI_MADT_TYPE_LOCAL_APIC:
            g_registry.cpu_info[g_registry.cpu_count].cpu_id = ((ACPI_MADT_LOCAL_APIC *)madt_subtable)->ProcessorId;
            g_registry.cpu_info[g_registry.cpu_count].local_apic_id = ((ACPI_MADT_LOCAL_APIC *)madt_subtable)->Id;
            g_registry.cpu_info[g_registry.cpu_count].local_apic_flags = ((ACPI_MADT_LOCAL_APIC *)madt_subtable)->LapicFlags;
            g_registry.cpu_count++;
            break;
        case ACPI_MADT_TYPE_IO_APIC:
            g_registry.io_apic_addr = ((ACPI_MADT_IO_APIC *)madt_subtable)->Address;
            break;
        case ACPI_MADT_TYPE_INTERRUPT_OVERRIDE:
            break;
        case ACPI_MADT_TYPE_LOCAL_APIC_NMI:
            break;
        default:
            bmv_printf("Unknown subtable type = %x\n", madt_subtable->Type);
            break;
        }
    }

    bmv_printf("Local APIC address = 0x%lx\n", g_registry.local_apic_addr);
    bmv_printf("I/O APIC address = 0x%lx\n", g_registry.io_apic_addr);
    bmv_printf("CPU count = %u\n", g_registry.cpu_count);

    // Find "FACP": Fixed ACPI Description Table (FADT)
    // https://wiki.osdev.org/FADT
    // ACPI_TABLE_FADT *fadt_table = (ACPI_TABLE_FADT *)acpi_find_sdt(rsdt_table, "FACP");

    // Find "HPET": High Precision Event Timer (HPET)
    // uintptr_t h2 = acpi_find_sdt(rsdt_table, "HPET");

    // Find "MCFG": PCI Memory Mapped Configuration table.
    ACPI_TABLE_MCFG *mcfg_table = (ACPI_TABLE_MCFG *)acpi_find_sdt(rsdt_table, "MCFG");

    // Go to the subtables and parse them.
    offset = sizeof(*mcfg_table);
    while (offset < mcfg_table->Header.Length)
    {
        ACPI_MCFG_ALLOCATION *mcfg_subtable = (ACPI_MCFG_ALLOCATION *)((uintptr_t)mcfg_table + offset);
        offset += sizeof(*mcfg_subtable);

        // bmv_printf("mcfg_subtable->Address = 0x%x\n", mcfg_subtable->Address);
        // bmv_printf("mcfg_subtable->PciSegment = 0x%x\n", mcfg_subtable->PciSegment);
        // bmv_printf("mcfg_subtable->StartBusNumber = 0x%x\n", mcfg_subtable->StartBusNumber);
        // bmv_printf("mcfg_subtable->EndBusNumber = 0x%x\n", mcfg_subtable->EndBusNumber);
    }
}