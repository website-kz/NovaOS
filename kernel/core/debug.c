#include <stdarg.h>
#include "vga.h"

void debug_log(const char* msg) {
    puts("[DEBUG] ");
    puts(msg);
    puts("\n");
}