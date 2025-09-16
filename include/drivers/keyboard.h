#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern "C" {
void keyboard_init();
char keyboard_getchar();
int keyboard_available();
void keyboard_handler();
}

#endif
