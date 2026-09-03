#include "stdio.h"
#include "stdint.h"
#include "cpu.h"
#include "bus.h"

void initilize_cpu(Cpu *cpu)
{
    cpu->A = 0x01;
    cpu->F = 0xb0;
    cpu->B = 0x00;
    cpu->C = 0x13;
    cpu->D = 0x00;
    cpu->E = 0xD8;
    cpu->H = 0x01;
    cpu->L = 0x4D;
    cpu->SP = 0xFFFE;
    cpu->PC = 0x0100;
}

void print_cpu(Cpu* cpu)
{
    printf("PC: %.4x \n", cpu->PC);
    printf("SP: %.4x \n", cpu->SP);
    printf("A: %.2x F: %.2x \n", cpu->A, cpu->F);
    printf("B: %.2x C: %.2x \n", cpu->B, cpu->C);
    printf("D: %.2x E: %.2x \n", cpu->D, cpu->E);
    printf("H: %.2x L: %.2x \n", cpu->H, cpu->L);
}

uint8_t get_8b_register(Cpu *cpu, Register reg)
{
    switch (reg)
    {
    case A:
        return cpu->A;
        break;
    case F:
        return cpu->F;
        break;
    case B:
        return cpu->B;
        break;
    case C:
        return cpu->C;
        break;
    case D:
        return cpu->D;
        break;
    case E:
        return cpu->E;
        break;
    case H:
        return cpu->H;
        break;
    case L:
        return cpu->L;
        break;

    default:
        break;
    }
    return 0;
}

uint16_t get_16b_register(Cpu *cpu, Register reg)
{
    uint16_t res = 0;
    switch (reg)
    {
    case AF:
        res = cpu->A;
        res <<= 8;
        res |= cpu->F;
        return res;
        break;
    case BC:
        res = cpu->B;
        res <<= 8;
        res |= cpu->C;
        return res;
        break;
    case DE:
        res = cpu->D;
        res <<= 8;
        res |= cpu->E;
        return res;
        break;
    case HL:
        res = cpu->H;
        res <<= 8;
        res |= cpu->L;
        return res;
        break;
    case SP:
        return cpu->SP;
        break;
    case PC:
        return cpu->PC;
        break;

    default:
        break;
    }
    return res;
}

int64_t cpu_step(Cpu *cpu, Cartridge *cartridge)
{
    uint16_t pc_address = get_16b_register(cpu, PC);
    uint8_t opcode = get_address(pc_address, cartridge);
    [[maybe_unused]] uint64_t cycles = 0;

    printf("PC=%.4x OPCODE=%.2x \n", pc_address, opcode);
    cpu->PC++;

    if(opcode == 0x00)
    {
        cycles = 4;
        return cycles;
    } else if (opcode == 0xC3)
    {
        /* JP with 2 argument bytes */
        uint16_t jp_address = 0;
        uint8_t jp_address_byte_1 = get_address(get_16b_register(cpu, PC), cartridge);
        cpu->PC++;
        uint8_t jp_address_byte_2 = get_address(get_16b_register(cpu, PC), cartridge);
        jp_address = jp_address_byte_2;
        jp_address <<= 8;
        jp_address |= jp_address_byte_1;
        cpu->PC = jp_address;
        cycles = 16;
        return cycles;
    }
    printf("Unsopported opcode \n");
    return -1;
}
