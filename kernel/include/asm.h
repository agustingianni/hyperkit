#ifndef ASM_H_
#define ASM_H_

static inline void halt(void)
{
    __asm__ __volatile__("hlt");
}

static inline void cr0_set(unsigned int value)
{
    __asm__ __volatile__("mov %0, %%cr0"
                         :
                         : "r"(value));
}

static inline void cr1_set(unsigned int value)
{
    __asm__ __volatile__("mov %0, %%cr1"
                         :
                         : "r"(value));
}

static inline void cr2_set(unsigned int value)
{
    __asm__ __volatile__("mov %0, %%cr2"
                         :
                         : "r"(value));
}

static inline void cr3_set(unsigned int value)
{
    __asm__ __volatile__("mov %0, %%cr3"
                         :
                         : "r"(value));
}

static inline void cr4_set(unsigned int value)
{
    __asm__ __volatile__("mov %0, %%cr4"
                         :
                         : "r"(value));
}

static inline unsigned int cr0_get(void)
{
    unsigned int ret = 0;
    __asm__ __volatile__("movl %%cr0, %0"
                         : "=r"(ret));
    return ret;
}

static inline unsigned int cr1_get(void)
{
    unsigned int ret = 0;
    __asm__ __volatile__("movl %%cr1, %0"
                         : "=r"(ret));
    return ret;
}

static inline unsigned int cr2_get(void)
{
    unsigned int ret = 0;
    __asm__ __volatile__("movl %%cr2, %0"
                         : "=r"(ret));
    return ret;
}

static inline unsigned int cr3_get(void)
{
    unsigned int ret = 0;
    __asm__ __volatile__("movl %%cr3, %0"
                         : "=r"(ret));
    return ret;
}

static inline unsigned int cr4_get(void)
{
    unsigned int ret = -1;
    __asm__ __volatile__("movl %%cr4, %0"
                         : "=r"(ret));
    return ret;
}

static inline unsigned eflags_get(void)
{
    unsigned eflags;

    __asm__ __volatile__(
        "pushfl\n"
        "popl %0"
        : "=m"(eflags));

    return eflags;
}

static inline void eflags_set(unsigned eflags)
{
    __asm__ __volatile__(
        "pushl %0\n"
        "popfl"
        :
        : "m"(eflags)
        : "cc", "memory");
}

static inline void lldt(unsigned short sel)
{
    __asm__ __volatile__("lldt %0"
                         :
                         : "r"(sel));
}

static inline void ltr(unsigned short sel)
{
    __asm__ __volatile__("ltr %0"
                         :
                         : "r"(sel));
}

static inline void lidt(void *base, uint16_t size)
{
    struct
    {
        uint16_t length;
        void *base;
    } __attribute__((packed)) IDTR = {size, base};

    __asm__ __volatile__("lidt %0"
                         :
                         : "m"(IDTR));
}

static inline void sti()
{
    __asm__ __volatile__("sti");
}

static inline void cli()
{
    __asm__ __volatile__("cli");
}

#endif /*ASM_H_*/
