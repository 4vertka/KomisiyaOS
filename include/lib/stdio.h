#ifndef STDIO_H
#define STDIO_H

#define _STDIO_H 1

#include <stdarg.h>

#define EOF (-1)

extern "C" {
int printf(const char *format, ...);
// int sprintf(char *str, const char *format, ...);
void putchar(char c);
void puts(const char *str);
static int vsprintf(char *str, const char *format, va_list args);
}

#endif
