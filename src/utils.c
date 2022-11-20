#include "traceroute.h"

#include <sys/time.h>

void *ft_memset(void *b, int c, size_t len) {
    unsigned char *str = b;

    while (len-- > 0) {
        *str++ = (unsigned char)c;
    }

    return b;
}

void *ft_memcpy(void *dst, const void *src, size_t n) {
    char            *d;
    const char      *s;
    size_t          i = 0;

    d = dst;
    s = src;
    if (d != 0 && s != 0) {
        while (i < n && s[i]) {
            d[i] = s[i];
            i++;
        }
    }
    d[i] = '\0';
    return (dst);
}

int ft_strcmp(const char *s1, const char *s2) {
    size_t  index = 0;

    while (s1[index])
    {
        if (s1[index] != s2[index])
            return ((unsigned char)s1[index] - (unsigned char)s2[index]);
        index++;
    }
    return (0);
}

int ft_atoi(const char *str) {
    int                 factor;
    unsigned long long  result;
    unsigned long long  max;
    int                 i;

    max = 9223372036854775807;
    result = 0;
    i = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '+' && str[i + 1] != '+' && str[i + 1] != '-')
        i++;
    factor = (str[i] == '-') ? -1 : 1;
    if (str[i] == '-')
        i++;
    while (str[i] >= '0' && str[i] <= '9') {
        if (factor == -1 && result > max)
            return (0);
        if (result > max)
            return (-1);
        result = result * 10 + str[i] - '0';
        i++;
    }
    return (factor * (int)result);
}

int is_number(const char *s) {
    int i = 0;

    while (s[i]) {
        if (s[i] < 48 || s[i] > 57) {
            return (0);
        }
        i++;
    }

    return (1);
}

double time_in_ms() {
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0);
}
