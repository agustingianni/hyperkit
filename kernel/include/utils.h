#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#define LOBYTE(value) ((uint8_t)(uint16_t)(value))
#define HIBYTE(value) ((uint8_t)(((uint16_t)(value)) >> 8))
#define LOWORD(value) ((uint16_t)(uint32_t)(value))
#define HIWORD(value) ((uint16_t)(((uint32_t)(value)) >> 16))
#define LODWORD(value64) ((uint32_t)(uint64_t)(value64))
#define HIDWORD(value64) ((uint32_t)(((uint64_t)(value64)) >> 32))

#define SET_BIT(target, bit) ((target) |= (bit))
#define CLEAR_BIT(target, bit) ((target) &= ~(bit))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#endif /* UTILS_H_ */