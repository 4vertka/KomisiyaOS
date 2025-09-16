#include "drivers/keyboard.h"
#include "kernel/idt.h"
#include "kernel/io.h"
#include "lib/stdio.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile int buffer_start = 0;
static volatile int buffer_end = 0;

static char keyboard_map[128] = {
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

static bool shift_pressed = false;
static bool caps_lock = false;

static void buffer_key(char key) {
  int next_end = (buffer_end + 1) % KEYBOARD_BUFFER_SIZE;
  if (next_end != buffer_start) {
    keyboard_buffer[buffer_end] = key;
    buffer_end = next_end;
  }
}

extern "C" void keyboard_handler() {
  uint8_t scancode = inb(KEYBOARD_DATA_PORT);

  if (scancode & 0x80) {
    scancode &= 0x7F;

    if (scancode == 0x2A || scancode == 0x36) {
      shift_pressed = false;
    }
    return;
  }

  switch (scancode) {
  case 0x2A:
  case 0x36:
    shift_pressed = true;
    break;

  case 0x3A:
    caps_lock = !caps_lock;
    break;

  default:
    if (scancode < 128) {
      char key = keyboard_map[scancode];
      if (key) {
        if (key >= 'a' && key <= 'z') {
          if (shift_pressed ^ caps_lock) {
            key = key - 'a' + 'A';
          }
        } else if (shift_pressed) {
          switch (key) {
          case '1':
            key = '!';
            break;
          case '2':
            key = '@';
            break;
          case '3':
            key = '#';
            break;
          case '4':
            key = '$';
            break;
          case '5':
            key = '%';
            break;
          case '6':
            key = '^';
            break;
          case '7':
            key = '&';
            break;
          case '8':
            key = '*';
            break;
          case '9':
            key = '(';
            break;
          case '0':
            key = ')';
            break;
          case '-':
            key = '_';
            break;
          case '=':
            key = '+';
            break;
          case '[':
            key = '{';
            break;
          case ']':
            key = '}';
            break;
          case ';':
            key = ':';
            break;
          case '\'':
            key = '"';
            break;
          case '`':
            key = '~';
            break;
          case '\\':
            key = '|';
            break;
          case ',':
            key = '<';
            break;
          case '.':
            key = '>';
            break;
          case '/':
            key = '?';
            break;
          }
        }

        buffer_key(key);
      }
    }
    break;
  }
}

extern "C" void keyboard_init() {
  buffer_start = 0;
  buffer_end = 0;
  shift_pressed = false;
  caps_lock = false;

  printf("Keyboard initialized\n");
}

extern "C" char keyboard_getchar() {
  while (buffer_start == buffer_end) {
    asm volatile("hlt");
  }

  char key = keyboard_buffer[buffer_start];
  buffer_start = (buffer_start + 1) % KEYBOARD_BUFFER_SIZE;
  return key;
}

extern "C" int keyboard_available() { return buffer_start != buffer_end; }
