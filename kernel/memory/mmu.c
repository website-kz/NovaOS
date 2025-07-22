// mmu.c — инициализация управления виртуальной памятью
#include <stdint.h>
#include <stddef.h>
#include "paging.h"
#include "debug.h"

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES     1024
#define PAGE_SIZE              0x1000

// 4 KB * 1024 * 1024 = 4 GB пространства

uint32_t *kernel_page_directory = (uint32_t*)0x9C000; // Память под каталог
uint32_t *first_page_table      = (uint32_t*)0x9D000; // Память под первую таблицу

void init_mmu() {
    debug_log("Initializing MMU...");

    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; // Present + RW
    }

    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
        kernel_page_directory[i] = 0x00000002; // RW, not present by default
    }

    // Подключаем первую таблицу в каталог
    kernel_page_directory[0] = ((uint32_t)first_page_table) | 3;

    // Загружаем каталог в CR3
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(kernel_page_directory));

    // Включаем бит PG в CR0
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));

    debug_log("MMU initialized. Paging enabled.");
}