#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static int test(int argument)
{
	return argument * 2;
}

int kernel_main(void)
{
	int ret = test(0x40404040 / 2);
	return ret;
}
