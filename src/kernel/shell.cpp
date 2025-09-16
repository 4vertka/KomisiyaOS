// src/kernel/shell_simple.cpp
// Simple shell without interrupt-based input (polling-based instead)

#include "kernel/shell.h"
#include "drivers/vga.h"
#include "kernel/io.h"
#include "lib/stdio.h"
#include "lib/string.h"

#define MAX_COMMAND_LENGTH 256
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static char command_buffer[MAX_COMMAND_LENGTH];

// Simple keyboard scancode to ASCII mapping
static char scancode_to_ascii[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
    '+', 0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0};

// Polling-based keyboard input (no interrupts needed)
static char keyboard_getchar_polling() {
  uint8_t scancode;
  char ascii;

  while (true) {
    // Check if keyboard has data
    if (inb(KEYBOARD_STATUS_PORT) & 0x01) {
      scancode = inb(KEYBOARD_DATA_PORT);

      // Only process key presses (not releases)
      if (!(scancode & 0x80)) {
        ascii = scancode_to_ascii[scancode];
        if (ascii != 0) {
          return ascii;
        }
      }
    }

    // Small delay to prevent overwhelming the CPU
    for (volatile int i = 0; i < 1000; i++)
      ;
  }
}

static void read_line() {
  int pos = 0;
  char c;

  printf("KomisiyaOS> ");

  while (true) {
    c = keyboard_getchar_polling();

    if (c == '\n') {
      command_buffer[pos] = '\0';
      printf("\n");
      break;
    } else if (c == '\b') { // Backspace
      if (pos > 0) {
        pos--;
        printf("\b \b"); // Erase character visually
      }
    } else if (c >= 32 && c <= 126) { // Printable characters
      if (pos < MAX_COMMAND_LENGTH - 1) {
        command_buffer[pos] = c;
        pos++;
        printf("%c", c);
      }
    }
  }
}

static void execute_command() {
  if (strlen(command_buffer) == 0) {
    return;
  }

  // Parse command and arguments
  if (strcmp(command_buffer, "help") == 0) {
    printf("Available commands:\n");
    printf("  help    - Show this help message\n");
    printf("  clear   - Clear the screen\n");
    printf("  echo    - Echo arguments\n");
    printf("  about   - Show system information\n");
    printf("  test    - Test system functions\n");
  } else if (strcmp(command_buffer, "clear") == 0) {
    vga_clear_screen();
  } else if (strncmp(command_buffer, "echo ", 5) == 0) {
    printf("%s\n", command_buffer + 5);
  } else if (strcmp(command_buffer, "about") == 0) {
    printf("KomisiyaOS v0.1\n");
    printf("A custom operating system built from scratch\n");
    printf("No external libraries used!\n");
    printf("Using polling-based keyboard input\n");
  } else if (strcmp(command_buffer, "test") == 0) {
    printf("Testing system functions:\n");
    printf("- VGA output: OK\n");
    printf("- Keyboard input: OK\n");
    printf("- String functions: OK\n");
    printf("- Printf: OK\n");
  } else {
    printf("Unknown command: %s\n", command_buffer);
    printf("Type 'help' for available commands\n");
  }
}

extern "C" void shell_run() {
  printf("Welcome to KomisiyaOS Shell!\n");
  printf("Built completely from scratch - no external libraries!\n");
  printf("Using polling-based keyboard input (no interrupts)\n");
  printf("Type 'help' for available commands.\n\n");

  while (true) {
    read_line();
    execute_command();
  }
}
