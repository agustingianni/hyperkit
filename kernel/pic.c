#include "io.h"
#include "pic.h"
#include "bmv.h"

void init_pic()
{
    bmv_printf("Remapping hardware interrupts.\n");

    // Send ICW1.
    outb(PIC_MASTER_CMD, 0x11);
    outb(PIC_SLAVE_CMD, 0x11);

    // Remap hardware interrupt so they don't collide with software interrupts.
    outb(PIC_MASTER_IMR, 0x20);
    outb(PIC_SLAVE_IMR, 0x28);

    // Setup the connection between the two PICs (cascading).
    outb(PIC_MASTER_IMR, 0x04);
    outb(PIC_SLAVE_IMR, 0x02);

    // Set MODE_8086 & !AUTO_EIO.
    outb(PIC_MASTER_IMR, 0x01);
    outb(PIC_SLAVE_IMR, 0x01);

    // Disable all IRQs on master & slave PIC by masking the IMR.
    outb(PIC_MASTER_IMR, 0xff);
    outb(PIC_SLAVE_IMR, 0xff);

    // TODO: This shuold not be here once we enable APIC.
    outb(PIC_MASTER_IMR, 0);
    outb(PIC_SLAVE_IMR, 0);

    bmv_printf("Masking hardware interrupts.\n");
}

void pic_send_eoi(int line)
{
    if (line >= 8)
        outb(PIC_SLAVE_CMD, 0x20);

    outb(PIC_MASTER_CMD, 0x20);
}
