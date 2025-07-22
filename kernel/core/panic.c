// panic.c — обработка критических ошибок ядра
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "debug.h"
#include "reboot.h"

#define PANIC_COLOR 0x4F // Красный фон, белый текст

static bool panic_triggered = false;

void panic(const char *message) {
    if (panic_triggered) {
        // Не повторяем панику
        while (1) __asm__ __volatile__("hlt");
    }

    panic_triggered = true;

    // Отключаем прерывания
    __asm__ __volatile__("cli");

    // Устанавливаем цвет для вывода
    vga_setcolor(PANIC_COLOR);

    puts("\n\n*** KERNEL PANIC ***\n");
    puts("Reason: ");
    puts(message);
    puts("\n\nSystem halted.");

    debug_log("PANIC: ");
    debug_log(message);

    // Здесь можно записывать журнал в RAM или в видеомем
    // log_to_memory(message);

    // Если в будущем будет watchdog — он перезагрузит систему.
    while (1) {
        __asm__ __volatile__("hlt");
    }
}