#include "lib/string.h"

extern "C" {

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

char *strcpy(char *dest, const char *src) {
  char *res = &dest[0];
  for (int i = 0; (dest[i] = src[i]); i++) {
  }
  return res;
}

char *strncpy(char *dest, const char *src, size_t n) {
  size_t i = 0;
  while (i < n && src[i]) {
    dest[i] = src[i];
    i++;
  }
}

int strcmp(const char *str1, const char *str2) {
  while (*str1 && (*str1 == *str2)) {
    str1++;
    str2++;
  }
  return *str1 - *str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
  size_t i = 0;
  while (*str1 && (*str1 == *str2) && i < n - 1) {
    str1++;
    str2++;
    i++;
  }
  return *str1 - *str2;
}

char *strcat(char *dest, const char *src) {
  char *a = dest;
  while (*a)
    a++;
  while ((*a++ = *src++))
    continue;
  return dest;
}

char *strchr(const char *str, int c) {
  while (*str != c && *str)
    str++;
  return *str == c ? (char *)str : 0;
}

void *memset(void *ptr, int value, size_t num) {
  unsigned char *a = (unsigned char *)ptr;
  while (num--)
    *a++ = (unsigned char)value;
  return ptr;
}

void *memcpy(void *dest, const void *source, size_t num) {
  unsigned char *dst = (unsigned char *)dest;
  const unsigned char *src = (const unsigned char *)source;
  while (num--)
    *dst++ = *src++;
  return dest;
}

int memcmp(const void *ptr1, const void *ptr2, size_t num) {
  const unsigned char *s1;
  const unsigned char *s2;
  s1 = (const unsigned char *)ptr1;
  s2 = (const unsigned char *)ptr2;
  char count = 0;
  for (size_t i = 0; i < num; i++) {
    if (s1[i] != s2[i]) {
      count = s1[i] - s2[i];
      break;
    }
  }
  return count;
}
}
