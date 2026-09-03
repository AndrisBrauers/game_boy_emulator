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
    A, F, B, C, D, E, H, L, AF, BC, DE, HL, PC, SP
} Register;


void initilize_cpu(Cpu *cpu);
void print_cpu(Cpu* cpu);
uint8_t get_8b_register(Cpu *cpu, Register reg);
uint16_t get_16b_register(Cpu *cpu, Register reg);
int64_t cpu_step(Cpu *cpu, Cartridge *cartridge);
