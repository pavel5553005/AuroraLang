#include "Syscalls.h"

uint8_t get_rx(uint8_t reg) { return ((reg >> 4) & 0x0F); }

uint8_t get_ry(uint8_t reg) { return (reg & 0x0F); }

void op_nop(VM* vm) { }

void op_halt(VM* vm) { exit(0); }

void op_mov_1(VM* vm)
{
    uint8_t reg = *vm->ip++;

    vm->r[get_rx(reg)] = vm->r[get_ry(reg)];
}

void op_mov_2(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->r[reg] = vm->br;
}

void op_mov_3(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->r[reg] = vm->tbr;
}

void op_mov_4(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->r[reg] = vm->sp;
}

void op_mov_5(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->r[reg] = vm->bp;
}

void op_mov_6(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t v = vm->r[reg];
    vm->br = v;
}

void op_mov_7(VM* vm)
{
    vm->br = vm->tbr;
}

void op_mov_8(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->tbr = vm->r[reg];
}

void op_mov_9(VM* vm)
{
    vm->tbr = vm->br;
}

void op_mov_10(VM* vm)
{
    uint8_t reg = *vm->ip++;

    uint64_t val = *(uint64_t*)vm->ip;
    vm->ip += 8;
    vm->r[reg] = val;
}

void op_mov_11(VM* vm)
{
    uint8_t reg = *vm->ip++;

    uint64_t addr = *(uint64_t*)vm->ip;
    vm->ip += 8;

    vm->r[reg] = *(uint64_t*)(get_mem(vm, addr));
}

void op_mov_12(VM* vm)
{
    uint64_t addr = *(uint64_t*)vm->ip;
    vm->ip += 8;

    uint8_t reg = *vm->ip++;

    *(uint64_t*)(get_mem(vm, addr)) = vm->r[reg];
}

void op_mov_13(VM* vm)
{
    uint64_t addr = *(uint64_t*)vm->ip;
    vm->ip += 8;

    *(uint8_t*)(get_mem(vm, addr)) = vm->br;
}

void op_mov_14(VM* vm)
{
    uint64_t addr = *(uint64_t*)vm->ip;
    vm->ip += 8;

    *(uint16_t*)(get_mem(vm, addr)) = vm->tbr;
}

void op_mov_15(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    vm->r[rx] = *(uint64_t*)(get_mem(vm, vm->r[ry]));
}

void op_mov_16(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    *(uint64_t*)(get_mem(vm, vm->r[rx])) = vm->r[ry];
}

void op_mov_17(VM* vm)
{
    uint8_t reg = *vm->ip++;
    *(uint8_t*)(get_mem(vm, vm->r[reg])) = vm->br;
}

void op_mov_18(VM* vm)
{
    uint8_t reg = *vm->ip++;
    *(uint16_t*)(get_mem(vm, vm->r[reg])) = vm->tbr;
}

void op_mov_19(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    uint64_t offset = *(uint64_t*)vm->ip;
    vm->ip += 8;

    vm->r[rx] = *(uint64_t*)(get_mem(vm, offset + vm->r[ry]));
}

void op_mov_20(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    uint64_t offset = *(uint64_t*)vm->ip;
    vm->ip += 8;

    *(uint64_t*)(get_mem(vm, vm->r[rx] + offset)) = vm->r[ry];
}

void op_mov_21(VM* vm)
{
    uint8_t reg = *vm->ip++;
    
    uint64_t offset = *(uint64_t*)vm->ip;
    vm->ip += 8;

    *(uint8_t*)(get_mem(vm, vm->r[reg] + offset)) = vm->br;
}

void op_mov_22(VM* vm)
{
    uint8_t reg = *vm->ip++;
    
    uint64_t offset = *(uint64_t*)vm->ip;
    vm->ip += 8;

    *(uint16_t*)(get_mem(vm, vm->r[reg] + offset)) = vm->tbr;
}


void op_push_1(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->sp -= 8;

    *(uint64_t*)(get_mem(vm, vm->sp)) = vm->r[reg];
}

void op_push_2(VM* vm)
{
    vm->sp--;

    *(uint8_t*)(get_mem(vm, vm->sp)) = vm->br;
}

void op_push_3(VM* vm)
{
    vm->sp -= 2;

    *(uint16_t*)(get_mem(vm, vm->sp)) = vm->tbr;
}

void op_pop_1(VM* vm)
{
    uint8_t reg = *vm->ip++;

    vm->r[reg] = *(uint64_t*)(get_mem(vm, vm->sp));
    vm->sp += 8;
}

void op_pop_2(VM* vm)
{
    vm->br = *(uint8_t*)(get_mem(vm, vm->sp));
    vm->sp++;
}

void op_pop_3(VM* vm)
{
    vm->tbr = *(uint16_t*)(get_mem(vm, vm->sp));
    vm->sp += 2;
}

void op_alloc(VM* vm)
{
   // Как нибудь в следующей раз
   printf("Attempt to call alloc\n");
   exit(1);
}

void op_free(VM* vm)
{
    // Как нибудь в следующей раз
    printf("Attempt to call free\n");
    exit(1);
}

void op_and(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    vm->r[rx] &= vm->r[ry];
}

void op_or(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    vm->r[rx] |= vm->r[ry];
}

void op_xor(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    vm->r[rx] ^= vm->r[ry];
}

void op_not(VM* vm)
{
    uint8_t reg = *vm->ip++;

    vm->r[reg] = ~vm->r[reg];
}

void op_shl(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    vm->r[rx] <<= vm->r[ry];
}

void op_shr(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);

    vm->r[rx] >>= vm->r[ry];
}

void op_add(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    
    vm->r[rx] += vm->r[ry];
}

void op_sub(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    
    vm->r[rx] -= vm->r[ry];
}

void op_mul(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    
    vm->r[rx] *= vm->r[ry];
}

void op_div(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    if (vm->r[ry] == 0)
    {
        vm->r[rx] = 0;
        return;
    }
    vm->r[rx] /= vm->r[ry];
}

void op_gre(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    
    vm->r[rx] = vm->r[rx] > vm->r[ry];
}

void op_les(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    
    vm->r[rx] = vm->r[rx] < vm->r[ry];
}

void op_eq(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint8_t rx = get_rx(reg);
    uint8_t ry = get_ry(reg);
    
    vm->r[rx] = vm->r[rx] == vm->r[ry];
}

void op_jmp(VM* vm)
{
    uint8_t reg = *vm->ip++;

    vm->ip = get_mem(vm, vm->r[reg] + MEM_SIZE / 4);
}

void op_jnz(VM* vm)
{
    uint8_t reg = *vm->ip++;

    if (vm->r[0])
    {
        vm->ip = get_mem(vm, vm->r[reg] + MEM_SIZE / 4);
    }
}

void op_jz(VM* vm)
{
    uint8_t reg = *vm->ip++;

    if (!(vm->r[0]))
    {
        vm->ip = get_mem(vm, vm->r[reg] + MEM_SIZE / 4);
    }
}

void op_syscall(VM* vm)
{
    uint8_t call = *vm->ip++;
    syscalls[call](vm);
}

void op_mov_23(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->r[reg] = vm->fbr;
}

void op_mov_24(VM* vm)
{
    vm->bp = vm->fbr;
}

void op_mov_25(VM* vm)
{
    vm->tbr = vm->fbr;
}

void op_mov_26(VM* vm)
{
    uint8_t reg = *vm->ip++;
    vm->fbr = vm->r[reg];
}

void op_mov_27(VM* vm)
{
    vm->fbr = vm->bp;
}

void op_mov_28(VM* vm)
{
    vm->fbr = vm->tbr;
}

void op_mov_29(VM* vm)
{
    uint64_t addr = *(uint64_t*)vm->ip;
    vm->ip += 8;

    *(uint32_t*)(get_mem(vm, addr)) = vm->tbr;
}

void op_mov_30(VM* vm)
{
    uint8_t reg = *vm->ip++;
    *(uint32_t*)(get_mem(vm, vm->r[reg])) = vm->fbr;
}

void op_mov_31(VM* vm)
{
    uint8_t reg = *vm->ip++;
    uint64_t offset = *(uint64_t*)vm->ip;
    *(uint32_t*)(get_mem(vm, vm->r[reg] + offset)) = vm->bp;
}

void op_push(VM* vm)
{
    vm->sp -= 8;

    *(uint32_t*)(get_mem(vm, vm->sp)) = vm->fbr;
}

void op_pop(VM* vm)
{
    vm->fbr = *(uint32_t*)(get_mem(vm, vm->sp));
    vm->sp += 8;
}

void op_addf(VM* vm)
{
    uint8_t reg = *vm->ip++;

    float f1, f2;
    memcpy(&f1, &vm->r[get_rx(reg)], 4);
    memcpy(&f2, &vm->r[get_ry(reg)], 4);

    f1 += f2;
    memcpy(&vm->r[get_rx(reg)], &f1, 4);
}

void op_subf(VM* vm)
{
    uint8_t reg = *vm->ip++;

    float f1, f2;
    memcpy(&f1, &vm->r[get_rx(reg)], 4);
    memcpy(&f2, &vm->r[get_ry(reg)], 4);

    f1 -= f2;
    memcpy(&vm->r[get_rx(reg)], &f1, 4);
}

void op_mulf(VM* vm)
{
    uint8_t reg = *vm->ip++;

    float f1, f2;
    memcpy(&f1, &vm->r[get_rx(reg)], 4);
    memcpy(&f2, &vm->r[get_ry(reg)], 4);

    f1 *= f2;
    memcpy(&vm->r[get_rx(reg)], &f1, 4);
}

void op_divf(VM* vm)
{
    uint8_t reg = *vm->ip++;

    float f1, f2;
    memcpy(&f1, &vm->r[get_rx(reg)], 4);
    memcpy(&f2, &vm->r[get_ry(reg)], 4);

    f1 /= f2;
    memcpy(&vm->r[get_rx(reg)], &f1, 4);
}

void op_addd(VM* vm)
{
    uint8_t reg = *vm->ip++;

    double d1, d2;
    memcpy(&d1, &vm->r[get_rx(reg)], 8);
    memcpy(&d2, &vm->r[get_ry(reg)], 8);

    d1 += d2;
    memcpy(&vm->r[get_rx(reg)], &d1, 8);
}

void op_subd(VM* vm)
{
    uint8_t reg = *vm->ip++;

    double d1, d2;
    memcpy(&d1, &vm->r[get_rx(reg)], 8);
    memcpy(&d2, &vm->r[get_ry(reg)], 8);

    d1 -= d2;
    memcpy(&vm->r[get_rx(reg)], &d1, 8);
}

void op_muld(VM* vm)
{
    uint8_t reg = *vm->ip++;

    double d1, d2;
    memcpy(&d1, &vm->r[get_rx(reg)], 8);
    memcpy(&d2, &vm->r[get_ry(reg)], 8);

    d1 *= d2;
    memcpy(&vm->r[get_rx(reg)], &d1, 8);
}

void op_divd(VM* vm)
{
    uint8_t reg = *vm->ip++;

    double d1, d2;
    memcpy(&d1, &vm->r[get_rx(reg)], 8);
    memcpy(&d2, &vm->r[get_ry(reg)], 8);

    d1 /= d2;
    memcpy(&vm->r[get_rx(reg)], &d1, 8);
}

void op_cvtif(VM* vm)
{
    uint8_t reg = *vm->ip++;

    float f1;
    memcpy(&f1, &vm->r[reg], 4);

    vm->r[reg] = (uint64_t)f1;
}

void op_cvtid(VM* vm)
{
    uint8_t reg = *vm->ip++;

    double d1;
    memcpy(&d1, &vm->r[reg], 8);

    vm->r[reg] = (uint64_t)d1;
}

void op_cvtfi(VM* vm)
{
    uint8_t reg = *vm->ip++;

    uint64_t val = vm->r[reg];
    float f1 = (float)val;

    memcpy(&vm->r[reg], &f1, 4);
}

void op_cvtfd(VM* vm)
{
    uint8_t reg = *vm->ip++;

    double val;
    memcpy(&val, &vm->r[reg], 8);
    float f1 = (float)val;

    memcpy(&vm->r[reg], &f1, 4);
}

void op_cvtdi(VM* vm)
{
    uint8_t reg = *vm->ip++;

    uint64_t val = vm->r[reg];
    double d1 = (double)val;

    memcpy(&vm->r[reg], &d1, 8);
}

void op_cvtdf(VM* vm)
{
    uint8_t reg = *vm->ip++;

    float val;
    memcpy(&val, &vm->r[reg], 4);
    double d1 = (double)val;

    memcpy(&vm->r[reg], &d1, 8);
}

void (*operators[49])(VM* vm) = {
    op_nop,     // 0x00
    op_halt,    // 0x01
    op_mov_1,   // 0x02
    op_mov_2,   // 0x03
    op_mov_3,   // 0x04
    op_mov_4,   // 0x05
    op_mov_5,   // 0x06
    op_mov_6,   // 0x07
    op_mov_7,   // 0x08
    op_mov_8,   // 0x09
    op_mov_9,   // 0x0a
    op_mov_10,  // 0x0b
    op_mov_11,  // 0x0c
    op_mov_12,  // 0x0d
    op_mov_13,  // 0x0e
    op_mov_14,  // 0x0f
    op_mov_15,  // 0x10
    op_mov_16,  // 0x11
    op_mov_17,  // 0x12
    op_mov_18,  // 0x13
    op_mov_19,  // 0x14
    op_mov_20,  // 0x15
    op_mov_21,  // 0x16
    op_mov_22,  // 0x17
    op_push_1,  // 0x18
    op_push_2,  // 0x19
    op_push_3,  // 0x1a
    op_pop_1,   // 0x1b
    op_pop_2,   // 0x1c
    op_pop_3,   // 0x1d
    op_alloc,   // 0x1e
    op_free,    // 0x1f
    op_and,     // 0x20
    op_or,      // 0x21
    op_xor,     // 0x22
    op_not,     // 0x23
    op_shl,     // 0x24
    op_shr,     // 0x25
    op_add,     // 0x26
    op_sub,     // 0x27
    op_mul,     // 0x28
    op_div,     // 0x29
    op_gre,     // 0x2a
    op_les,     // 0x2b
    op_eq,      // 0x2c
    op_jmp,     // 0x2d
    op_jnz,     // 0x2e
    op_jz,      // 0x2f
    op_syscall  // 0x30
};
