#include "drivers/vga.h"

extern "C" void kernel_main() {
  vga_init();
  vga_clear_screen();
  vga_write_string("velcom to komosiya os");
}
