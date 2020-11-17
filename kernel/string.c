#include "string.h"

char *strcpy(char *dest, const char *src)
{
    char *tmp = dest;

    while ((*dest++ = *src++) != '\0')
        /* nothing */;
    return tmp;
}

int strcmp(const char *cs, const char *ct)
{
    signed char __res;

    while (1)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
    }
    return __res;
}

int strncmp(const char *cs, const char *ct, size_t count)
{
    signed char __res = 0;

    while (count)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count--;
    }
    return __res;
}

char *strchr(const char *s, int c)
{
    for (; *s != (char)c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *)s;
}

size_t strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        ;
    return sc - s;
}

void *memset(void *s, int c, size_t count)
{
    char *xs = s;

    while (count--)
    {
        *xs++ = c;
    }

    return s;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *tmp = dest;
    const char *s = src;

    while (count--)
        *tmp++ = *s++;
    return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
    char *tmp;
    const char *s;

    if (dest <= src)
    {
        tmp = dest;
        s = src;
        while (count--)
            *tmp++ = *s++;
    }
    else
    {
        tmp = dest;
        tmp += count;
        s = src;
        s += count;
        while (count--)
            *--tmp = *--s;
    }
    return dest;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}

char *strstr(const char *s1, const char *s2)
{
    int l1, l2;

    l2 = strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = strlen(s1);
    while (l1 >= l2)
    {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}

void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    while (n-- != 0)
    {
        if ((unsigned char)c == *p++)
        {
            return (void *)(p - 1);
        }
    }
    return NULL;
}

void *memmem(const void *big, size_t big_len, const void *little, size_t little_len)
{
    register const char *ph;
    register const char *pn;
    const char *plast;
    size_t n;

    if (little_len == 0)
    {
        return (void *)big;
    }

    if (big_len >= little_len)
    {
        ph = (const char *)big;
        pn = (const char *)little;
        plast = ph + (big_len - little_len);

        do
        {
            n = 0;
            while (ph[n] == pn[n])
            {
                if (++n == little_len)
                {
                    return (void *)ph;
                }
            }
        } while (++ph <= plast);
    }

    return NULL;
}