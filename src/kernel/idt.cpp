#include "kernel/idt.h"
#include "lib/string.h"

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

extern "C" void idt_load(uint32_t); // Assembly function

extern "C" void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel,
                             uint8_t flags) {
  idt[num].base_low = (base & 0xFFFF);
  idt[num].base_high = (base >> 16) & 0xFFFF;
  idt[num].selector = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}

extern "C" void idt_init() {
  idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
  idtp.base = (uint32_t)&idt;

  memset(&idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES);

  idt_load((uint32_t)&idtp);
}
