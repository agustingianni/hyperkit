#ifndef TIMER_H_
#define TIMER_H_

// Ticking rate of the 8254 chip.
#define CLOCK_TICK_RATE 1193182
#define FREQUENCY 1000
#define LATCH (CLOCK_TICK_RATE / FREQUENCY)

// PIT's IO addresses.
#define PIT_COUNTER_0_ADDR 0x40 
#define PIT_COUNTER_1_ADDR 0x41
#define PIT_COUNTER_2_ADDR 0x42
#define PIT_CTRL_WR_ADDR 0x43

/*
  * RW Read/Write:
  * RW1 RW0
  * 0   0 Counter Latch Command (see Read Operations)
  * 0   1 Read/Write least significant byte only.
  * 1   0 Read/Write most significant byte only.
  * 1   1 Read/Write least significant byte first, then most significant byte.
  */

#define PIT_CMD_LATCH (0 << 4)
#define PIT_CMD_RW_LSB (1 << 4)
#define PIT_CMD_RW_MSB (2 << 4)
#define PIT_CMD_RW_LSB_MSB (3 << 4)

/*
  * M Mode
  * 0 0 0 = Mode 0 (interrupt on terminal count)
  * 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
  * 0 1 0 = Mode 2 (rate generator)
  * 0 1 1 = Mode 3 (square wave generator)
  * 1 0 0 = Mode 4 (software triggered strobe)
  * 1 0 1 = Mode 5 (hardware triggered strobe)
  * 1 1 0 = Mode 6 (rate generator, same as 010b)
  * 1 1 1 = Mode 7 (square wave generator, same as 011b)
  */

#define PIT_CMD_MODE_0 (0 << 1)
#define PIT_CMD_MODE_1 (1 << 1)
#define PIT_CMD_MODE_2 (2 << 1)
#define PIT_CMD_MODE_3 (3 << 1)
#define PIT_CMD_MODE_4 (4 << 1)
#define PIT_CMD_MODE_5 (5 << 1)

void init_timer(void);

#endif /* TIMER_H_ */