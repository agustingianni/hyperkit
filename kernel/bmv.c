#include <stdarg.h>

#include "io.h"
#include "bmv.h"
#include "printf.h"

void bmv_putchar(char chr)
{
    if (chr == '\n')
        outl(BVM_CONSOLE_PORT, '\r');

    outl(BVM_CONSOLE_PORT, chr);
}

void bmv_print(const char *string)
{
    while (*string)
    {
        bmv_putchar(*string);
        string++;
    }
}

void bmv_println(const char *string)
{
    bmv_print(string);
    bmv_putchar('\n');
}

void bmv_printf(const char *format, ...)
{
    static char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    bmv_print(buffer);
}
