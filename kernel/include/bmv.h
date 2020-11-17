#ifndef BMV_H_
#define BMV_H_

#include "printf.h"

#define BVM_CONSOLE_PORT 0x220

void bmv_putchar(char chr);
void bmv_print(const char *string);
void bmv_println(const char *string);
void bmv_printf(const char *format, ...) PRINTFLIKE(1, 2);

#endif /* BMV_H_ */