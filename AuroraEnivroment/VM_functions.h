#include "Operators.h"

void init_vm(VM* vm)
{
    vm->mem = (uint8_t*)malloc(MEM_SIZE);
    uint32_t data = 0;
    uint32_t text = MEM_SIZE / 4;
    uint32_t stack = 2 * MEM_SIZE / 4;
    uint32_t heap = 3 * MEM_SIZE / 4;

    for (int i = 0; i < 8; i++)
    {
        vm->r[i] = 0;
    }
    vm->br = 0;
    vm->tbr = 0;
    vm->sp = heap;
    vm->bp = heap;

    vm->fbr = 0;

    vm->ip = vm->mem + text;
}

void free_vm(VM* vm)
{
    free(vm->mem);
}

void debug_print_memory(VM* vm)
{
    for (int i = MEM_SIZE / 4; i < MEM_SIZE /4 + 32; i++)
    {
        printf("%02x ", vm->mem[i]);
    }
}

void run_vm(VM* vm)
{
    while (vm->ip < vm->mem + 2 * MEM_SIZE / 4)
    {
        uint8_t opcode = *vm->ip++;
        if (opcode <= 48)
        {
            printf("  ↓  ↓  ↓  ↓  ↓  ↓\n");
            printf("Opcode: %02X at %ld\n", opcode, vm->ip - 1 - vm->mem - MEM_SIZE / 4);
            if (opcode == 0x01)
            {
                return;
            }
            operators[opcode](vm);
        }
        else
        {
            printf("Unknown opcode: %d\n", opcode);
            exit(1);
        }
    }
}
