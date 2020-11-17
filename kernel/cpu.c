#include <stdint.h>

#include "cpu.h"
#include "bmv.h"

static void cpuid(uint32_t in, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ volatile(
        "cpuid;"
        : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
        : "a"(in));
}

static void cpuid_features(uint32_t *ecx, uint32_t *edx)
{
    uint32_t eax = 0, ebx = 0;
    cpuid(1, &eax, &ebx, ecx, edx);
}

static void cpuid_extended_features(uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    uint32_t eax = 0;
    cpuid(7, &eax, ebx, ecx, edx);
}

void init_cpu()
{
    // https://code.woboq.org/linux/linux/arch/x86/kernel/cpu/intel.c.html#init_intel
    // Get CPU features.
    {
        uint32_t ecx, edx;
        cpuid_features(&ecx, &edx);
        bmv_printf("ecx = 0x%lx | edx = 0x%lx\n", ecx, edx);
    }

    // Gete CPU extended features.
    {
        uint32_t ebx, ecx, edx;
        cpuid_extended_features(&ebx, &ecx, &edx);
        bmv_printf("ebx = 0x%lx | ecx = 0x%lx | edx = 0x%lx\n", ebx, ecx, edx);
    }
}
