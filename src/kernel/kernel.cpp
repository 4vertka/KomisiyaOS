#include "drivers/vga.h"
#include "lib/stdio.h"

extern "C" void kernel_main() {
  vga_init();
  vga_clear_screen();
  printf("abiba");

  // vga_write_string("velcom to komosiya os");
}
