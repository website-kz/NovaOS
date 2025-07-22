#include "panic.h"
#include "debug.h"

void kernel_main() {
    debug_log("NovaShell: Kernel starting...");
    
    init_memory();
    init_interrupts();
    
    debug_log("NovaShell: Kernel initialized successfully.");
    
    while (1) {
        // Idle loop
        __asm__ __volatile__("hlt");
    }
}