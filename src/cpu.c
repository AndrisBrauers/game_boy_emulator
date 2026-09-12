#include "stdio.h"
#include "stdint.h"
#include "cpu.h"
#include "bus.h"
#include "decoder.h"
#include "instructions.h"

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
    cpu->IME = OFF;
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
    case REG_A:
        return cpu->A;
        break;
    case REG_F:
        return cpu->F;
        break;
    case REG_B:
        return cpu->B;
        break;
    case REG_C:
        return cpu->C;
        break;
    case REG_D:
        return cpu->D;
        break;
    case REG_E:
        return cpu->E;
        break;
    case REG_H:
        return cpu->H;
        break;
    case REG_L:
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
    case REG_AF:
        res = cpu->A;
        res <<= 8;
        res |= cpu->F;
        return res;
        break;
    case REG_BC:
        res = cpu->B;
        res <<= 8;
        res |= cpu->C;
        return res;
        break;
    case REG_DE:
        res = cpu->D;
        res <<= 8;
        res |= cpu->E;
        return res;
        break;
    case REG_HL:
        res = cpu->H;
        res <<= 8;
        res |= cpu->L;
        return res;
        break;
    case REG_SP:
        return cpu->SP;
        break;
    case REG_PC:
        return cpu->PC;
        break;

    default:
        break;
    }
    return res;
}


uint8_t fetch_8b(Cpu *cpu, Memory *memory)
{
    uint8_t res = get_8b_val(get_16b_register(cpu, REG_PC), memory);
    cpu->PC++;
    return res;
}

uint16_t fetch_16b(Cpu *cpu, Memory *memory)
{
    uint16_t res = 0;
    uint8_t byte_1 = fetch_8b(cpu, memory);
    uint8_t byte_2 = fetch_8b(cpu, memory);
    res = byte_2;
    res <<= 8;
    res |= byte_1;
    return res;
}

void put_reg(Cpu *cpu, Register reg, uint16_t val)
{
    switch (reg)
    {
    case REG_A:
        cpu->A = val;
        break;
    case REG_F:
        cpu->F = (val & 0x00F0); /*because last 3 bits are used for flags*/
        break;
    case REG_B:
        cpu->B = val;
        break;
    case REG_C:
        cpu->C = val;
        break;
    case REG_D:
        cpu->D = val;
        break;
    case REG_E:
        cpu->E = val;
        break;
    case REG_H:
        cpu->H = val;
        break;
    case REG_L:
        cpu->L = val;
        break;
    case REG_AF:
        cpu->A = (val >> 8);
        cpu->F = (val & 0x00F0);
        break;
    case REG_BC:
        cpu->B = (val >> 8);
        cpu->C = val;
        break;
    case REG_DE:
        cpu->D = (val >> 8);
        cpu->E = val;
        break;
    case REG_HL:
        cpu->H = (val >> 8);
        cpu->L = val;
        break;
    case REG_SP:
        cpu->SP = val;
        break;
    case REG_PC:
        cpu->PC = val;
        break;
    default:
        break;
    }
}

void stack_push_16b(Cpu *cpu, Memory *memory, uint16_t val)
{
    uint8_t byte_1 = (val >> 8);
    uint8_t byte_2 = (val & 0x00FF);
    cpu->SP--;
    put_8b_val(cpu->SP, memory, byte_1);
    cpu->SP--;
    put_8b_val(cpu->SP, memory, byte_2);
}

uint16_t stack_pop_16b(Cpu *cpu, Memory *memory)
{
    uint16_t res = 0;
    uint8_t byte_1 = get_8b_val(cpu->SP, memory);
    cpu->SP++;
    uint8_t byte_2 = get_8b_val(cpu->SP, memory);
    cpu->SP++;
    res = byte_2;
    res <<= 8;
    res |= byte_1;
    return res;
}

void set_flag(Cpu *cpu, Flag flag, State state)
{
    uint8_t mask;

    switch (flag)
    {
        case FLAG_Z:
            mask = 0x80;
            break;
        case FLAG_N:
            mask = 0x40;
            break;
        case FLAG_H:
            mask = 0x20;
            break;
        case FLAG_C:
            mask = 0x10;
            break;
        case FLAG_ALL:
            mask = 0xF0;
            break;
        default:
            break;
    }
    if (state == ON)
    {
        cpu->F |= mask;
    } else {
        cpu->F &= ~mask;
    }
}

State read_flag(Cpu *cpu, Flag flag)
{
    uint8_t mask;
    switch (flag)
    {
        case FLAG_Z:
            mask = 0x80;
            break;
        case FLAG_N:
            mask = 0x40;
            break;
        case FLAG_H:
            mask = 0x20;
            break;
        case FLAG_C:
            mask = 0x10;
            break;
        case FLAG_ALL:
            mask = 0xF0;
            break;
        default:
            break;
    }
    return (cpu->F & mask) > 0 ? ON : OFF;
}

State check_for_zero(uint16_t val)
{
    return (val == 0) ? ON : OFF;
}
State check_for_carry(Cpu *cpu, uint16_t val_1, uint16_t val_2, Operation op)
{
    State res = OFF;
    switch (op)
    {
    case ADD:
        res = ((val_1 + val_2) > 0xFF) ? ON : OFF;
        break;
    case ADD_16B:
        res = ((uint32_t)val_1 + val_2 > 0xFFFF) ? ON : OFF; 
        break;
    case ADC:
        res = ((val_1 + val_2 + read_flag(cpu, FLAG_C)) > 0xFF) ? ON : OFF;
        break;
    case SUB:
        res = (val_1 < val_2) ? ON : OFF;
        break;
    case SBC:
        res = (val_1 < (val_2 + read_flag(cpu, FLAG_C))) ? ON : OFF;
        break;
    case RLCA:
    case RLA:
        res = ((val_1 & 0x80) != 0) ? ON : OFF;
        break;
    case RRCA:
    case RRA:
        res = ((val_1 & 0x01) != 0) ? ON : OFF;
        break;
    default:
        break;
    }
    return res;
}
State check_for_half_carry(Cpu *cpu, uint16_t val_1, uint16_t val_2, Operation op)
{
    State res = OFF;
    switch (op)
    {
    case ADD:
    case INC:
        res = ((((val_1 & 0x0F) + (val_2 & 0x0F)) & 0x10) == 0x10) ? ON : OFF;
        break;
    case ADD_16B:
        res = ((val_1 & 0x0FFF) + (val_2 & 0x0FFF) > 0x0FFF) ? ON : OFF;
    case ADC:
        res = ((val_1 & 0x0F) +(val_2 & 0x0F) + read_flag(cpu, FLAG_C) > 0x0F) ? ON : OFF;
        break;
    case SUB:
    case DEC:
        res = (((val_1 & 0x0F) < (val_2 & 0x0F))) ? ON : OFF;
        break;
    case SBC:
        res = (((val_1 & 0x0F) < ((val_2 & 0x0F) + read_flag(cpu, FLAG_C)) )) ? ON : OFF;
        break;
    default:
        break;
    }
    return res;
}

uint8_t check_condition(Cpu* cpu, Condition cond)
{
    if (cond == COND_Z)
    {
        return (read_flag(cpu, FLAG_Z) == ON) ? 1 : 0;
    }
    else if (cond == COND_NZ)
    {
        return (read_flag(cpu, FLAG_Z) == OFF) ? 1 : 0;
    }
    else if (cond == COND_C)
    {
        return (read_flag(cpu, FLAG_C) == ON) ? 1 : 0;
    }
    else if (cond == COND_NC)
    {
        return (read_flag(cpu, FLAG_C) == OFF) ? 1 : 0;
    } 
    else
    {
        return 1;
    }
}

int64_t cpu_step(Cpu *cpu, Memory *memory)
{
    uint16_t pc_address = get_16b_register(cpu, REG_PC);
    uint8_t opcode = get_8b_val(pc_address, memory);
    
    cpu->PC++;

    Instruction instruction_to_exe = (opcode == 0xCB) ? decode_cb_opcode(fetch_8b(cpu, memory)) 
                                                      : decode_base_opcode(opcode);
    int16_t cycles = instruction_to_exe.handler(cpu, memory, &instruction_to_exe);

    printf("PC=%.4x OPCODE=%.2x cycles=%i \n", pc_address, opcode, cycles);
    return cycles;
}

