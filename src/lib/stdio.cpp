#include "lib/stdio.h"
#include "drivers/vga.h"
#include "lib/string.h"
#include <limits.h>

void putchar(char c) { vga_putchar(c); }

static void itoa(int value, char *str, int base) {
  char *ptr = str;
  char *ptr1 = str;
  char tmp_char;
  int tmp_value;

  if (value < 0 && base == 10) {
    *ptr++ = '-';
    value = -value;
    str++;
    ptr1++;
  }

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "0123456789abcdef"[tmp_value - value * base];
  } while (value);

  *ptr-- = '\0';

  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
}

static void utoa(unsigned int value, char *str, int base) {
  char *ptr = str;
  char *ptr1 = str;
  char tmp_char;
  unsigned int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "0123456789abcdef"[tmp_value - value * base];
  } while (value);

  *ptr-- = '\0';

  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
}

static int vsprintf(char *str, const char *format, va_list args) {
  char *str_orig = str;

  while (*format) {
    if (*format == '%') {
      format++;
      switch (*format) {
      case 'c': {
        int c = va_arg(args, int);
        *str++ = (char)c;
        break;
      }
      case 's': {
        char *s = va_arg(args, char *);
        while (*s)
          *str++ = *s++;
        break;
      }
      case 'd': {
        int i = va_arg(args, int);
        char num_str[32];
        itoa(i, num_str, 10);
        char *p = num_str;
        while (*p)
          *str++ = *p++;
        break;
      }
      case 'u': {
        unsigned int i = va_arg(args, unsigned int);
        char num_str[32];
        utoa(i, num_str, 10);
        char *p = num_str;
        while (*p)
          *str++ = *p++;
        break;
      }
      case 'x': {
        unsigned int i = va_arg(args, unsigned int);
        char num_str[32];
        utoa(i, num_str, 16);
        char *p = num_str;
        while (*p)
          *str++ = *p++;
        break;
      }
      case '%':
        *str++ = '%';
        break;
      default:
        *str++ = '%';
        *str++ = *format;
        break;
      }
    } else {
      *str++ = *format;
    }
    format++;
  }
  *str = '\0';
  return str - str_orig;
}

int printf(const char *format, ...) {
  char buffer[1024];
  va_list args;
  va_start(args, format);
  int result = vsprintf(buffer, format, args);
  va_end(args);

  vga_write_string(buffer);
  return result;
}

void puts(const char *str) {
  vga_write_string(str);
  vga_putchar('\n');
}
