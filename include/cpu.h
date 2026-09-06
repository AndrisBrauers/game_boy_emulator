#pragma once
#include "stdint.h"
#include "cartridge.h"
typedef struct
{
    uint8_t A;
    uint8_t F;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    uint16_t SP;
    uint16_t PC;
} Cpu;

typedef enum {
    REG_A, REG_F, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_AF, REG_BC, REG_DE, REG_HL, REG_PC, REG_SP
} Register;

typedef enum {
    FLAG_Z, FLAG_N, FLAG_H, FLAG_C, FLAG_ALL
} Flag;

typedef enum {
    OFF, ON
} Flag_state;

typedef enum {
    ADD, INC, ADC, SUB, DEC, SBC, AND, XOR, OR, CP
} Operation;

void initilize_cpu(Cpu *cpu);
void print_cpu(Cpu* cpu);
uint8_t get_8b_register(Cpu *cpu, Register reg);
uint16_t get_16b_register(Cpu *cpu, Register reg);
void put_reg(Cpu *cpu, Register reg, uint16_t val);
void set_flag(Cpu *cpu, Flag flag, Flag_state state);
Flag_state read_flag(Cpu *cpu, Flag flag);
Flag_state check_for_carry(Cpu *cpu, uint16_t val_1, uint16_t val_2, Operation op);
Flag_state check_for_half_carry(Cpu *cpu, uint8_t val_1, uint8_t val_2, Operation op);
int64_t cpu_step(Cpu *cpu, Cartridge *cartridge);

