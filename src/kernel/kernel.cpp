#include "drivers/vga.h"
#include "kernel/shell.h"
#include "lib/stdio.h"

extern "C" void kernel_main() {
  vga_init();
  vga_clear_screen();

  printf("KomisiyaOS - Built from scratch!\n");
  printf("Initializing system...\n\n");

  shell_run();

  while (true) {
    asm volatile("hlt");
  }
}
