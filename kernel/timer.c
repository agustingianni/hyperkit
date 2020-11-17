#include "io.h"
#include "utils.h"
#include "timer.h"
#include "interrupts.h"

void init_timer(void)
{
    irq_disable();
    {
        outb(PIT_CTRL_WR_ADDR, PIT_CMD_RW_LSB_MSB | PIT_CMD_MODE_3);
        outb(PIT_COUNTER_0_ADDR, LOBYTE(LATCH));
        outb(PIT_COUNTER_0_ADDR, HIBYTE(LATCH));
    }
    irq_enable();
}
