#include "kernel/io.h"

extern "C" {

uint8_t inb(uint16_t port) {
  uint8_t result;
  asm volatile("inb %1, %0" : "=a"(result) : "dN"(port));
  return result;
}

void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

uint16_t inw(uint16_t port) {
  uint16_t result;
  asm volatile("inw %1, %0" : "=a"(result) : "dN"(port));
  return result;
}

void outw(uint16_t port, uint16_t value) {
  asm volatile("outw %1, %0" : : "dN"(port), "a"(value));
}
}
