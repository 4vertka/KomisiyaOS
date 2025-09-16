#ifndef STRING_H
#define STRING_H

#include <stddef.h>

extern "C" {
void *memset(void *ptr, int value, size_t num);
void *memcpy(void *dest, const void *src, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
}

#endif
