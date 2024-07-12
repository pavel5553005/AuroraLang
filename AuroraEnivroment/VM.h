#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Определение размеров сегментов памяти
#define MEM_SIZE 1024 * 1024 * 64
// Определение структуры виртуальной машины
typedef struct
{
    uint64_t r[8]; // Общие регистры r0 - r7
    uint8_t br; // 8-битный регистр
    uint16_t tbr; // 16-битный регистр
    uint64_t sp; // Указатель стека
    uint64_t bp; // Базовый указатель стека

    uint32_t fbr; // 32-битный регистр

    uint8_t* mem; // Память
    //
    // Расположение сегментов памяти
    // data text stack heap
    //

    uint8_t* ip;
} VM;

uint8_t* get_mem(VM* vm, uint64_t addr)
{
    if (addr >= MEM_SIZE || addr < 0)
    {
        printf("Error: Out of memory\n");
        exit(1);
    }
    return vm->mem + addr;
}
