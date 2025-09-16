#include "drivers/vga.h"

static uint16_t *vga_buffer = (uint16_t *)0xB8000;
static size_t vga_row = 0;
static size_t vga_column = 0;
static uint8_t vga_color = 0x07;

void vga_init() {
  vga_row = 0;
  vga_column = 0;
  vga_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  vga_clear_screen();
}

void vga_clear_screen() {
  for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga_buffer[i] = vga_entry(' ', vga_color);
  }
  vga_row = 0;
  vga_column = 0;
}

void vga_scroll() {
  for (size_t row = 1; row < VGA_HEIGHT; row++) {
    for (size_t col = 0; col < VGA_WIDTH; col++) {
      vga_buffer[(row - 1) * VGA_WIDTH + col] =
          vga_buffer[row * VGA_WIDTH + col];
    }
  }

  for (size_t col = 0; col < VGA_WIDTH; col++) {
    vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = vga_entry(' ', vga_color);
  }

  vga_row = VGA_HEIGHT - 1;
  vga_column = 0;
}

void vga_putchar(char c) {
  if (c == '\n') {
    vga_column = 0;
    if (++vga_row >= VGA_HEIGHT) {
      vga_scroll();
    }
    return;
  }

  if (c == '\r') {
    vga_column = 0;
    return;
  }

  if (c == '\b') {
    if (vga_column > 0) {
      vga_column--;
      vga_buffer[vga_row * VGA_WIDTH + vga_column] = vga_entry(' ', vga_color);
    }
    return;
  }

  if (c == '\t') {
    size_t tab_stop = (vga_column + 4) & ~3;
    if (tab_stop >= VGA_WIDTH) {
      vga_column = 0;
      if (++vga_row >= VGA_HEIGHT) {
        vga_scroll();
      }
    } else {
      vga_column = tab_stop;
    }
    return;
  }

  vga_buffer[vga_row * VGA_WIDTH + vga_column] = vga_entry(c, vga_color);

  if (++vga_column >= VGA_WIDTH) {
    vga_column = 0;
    if (++vga_row >= VGA_HEIGHT) {
      vga_scroll();
    }
  }
}

void vga_write_string(const char *str) {
  while (*str) {
    vga_putchar(*str);
    str++;
  }
}

void vga_set_color(uint8_t color) { vga_color = color; }

void vga_set_cursor_pos(size_t x, size_t y) {
  if (x < VGA_WIDTH && y < VGA_HEIGHT) {
    vga_column = x;
    vga_row = y;
  }
}
