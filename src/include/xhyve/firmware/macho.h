#pragma once

#include <stdint.h>

int macho_init(char *kernel_path, char *module_list, char *kernel_append);
uint64_t macho(void);
