#include "VM.h"

void call_open(VM* vm);
void call_read(VM* vm);
void call_write(VM* vm);
void call_close(VM* vm);

void call_write_line(VM* vm);

void call_get_system_time(VM* vm);

void (*syscalls[49])(VM* vm) = {
    call_open,           // 0x00
    call_read,           // 0x01
    call_write,          // 0x02
    call_close,          // 0x03
    call_write_line,     // 0x04
    call_get_system_time // 0x05
};

void call_open(VM* vm)
{
    printf("Error: Syscall not implemented: open\n");
}

void call_read(VM* vm)
{
    printf("Error: Syscall not implemented: read\n");
}

void call_write(VM* vm)
{
    printf("Error: Syscall not implemented: write\n");
}

void call_close(VM* vm)
{
    printf("Error: Syscall not implemented: close\n");
}

void call_write_line(VM* vm)
{
    char* str = vm->mem + vm->r[0];
    printf(str);
    printf("\n");
}

void call_get_system_time(VM* vm)
{
    printf("Error: Syscall not implemented: get_system_time\n");
}
