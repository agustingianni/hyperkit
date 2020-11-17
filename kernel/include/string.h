#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

char *strcpy(char *dest, const char *src);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
char *strchr(const char *s, int c);
size_t strlen(const char *s);
void *memset(void *s, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *cs, const void *ct, size_t count);
char *strstr(const char *s1, const char *s2);
void *memchr(const void *s, int c, size_t n);
void *memmem(const void *big, size_t big_len, const void *little, size_t little_len);
#endif /* _STRING_H_ */
