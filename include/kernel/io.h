#ifndef IO_H
#define IO_H

#include <stdint.h>

extern "C" {
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t value);
}

#endif
