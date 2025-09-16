#include "kernel/shell.h"
#include "drivers/vga.h"
#include "lib/stdio.h"
#include "lib/string.h"

#define MAX_COMMAND_LENGTH 256

static char command_buffer[MAX_COMMAND_LENGTH];

static void read_line() {}
