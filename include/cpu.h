#pragma once
#include "stdint.h"
#include "bus.h"
#include "cartridge.h"
#include "cpu.h"

typedef enum {
    NO_REG, REG_A, REG_F, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_AF, REG_BC, REG_DE, REG_HL, REG_PC, REG_SP, 
    MEM_C, MEM_HL, MEM_BC, MEM_DE, MEM_HL_INC, MEM_HL_DEC, IMM8, IMM16, REL8, MEM_A8, MEM_A16, REG_SP_E8
} Register;

typedef enum {
    FLAG_Z, FLAG_N, FLAG_H, FLAG_C, FLAG_ALL
} Flag;

typedef enum {
    OFF, ON
} State;

typedef enum {
    NO_OP, ADD, ADD_16B, INC, ADC, SUB, DEC, SBC, AND, XOR, OR, CP,
    RLCA, RRCA, RLA, RRA, DAA, SCF, CPL, CCF, LD, JR, JP, CALL, RET, RETI,
    DI, EI, RST, POP, PUSH,
    RLC, RRC, RL, RR, SLA, SRA, SWAP, SRL, BIT, RES, SET
} Operation;

typedef enum {
    NO_COND, COND_Z, COND_NZ, COND_C, COND_NC
} Condition;

typedef struct Cpu Cpu;
struct Cpu
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
    State IME;
};



void initilize_cpu(Cpu *cpu);
void print_cpu(Cpu* cpu);
uint8_t get_8b_register(Cpu *cpu, Register reg);
uint16_t get_16b_register(Cpu *cpu, Register reg);
uint8_t fetch_8b(Cpu* cpu, Memory* memory);
uint16_t fetch_16b(Cpu* cpu, Memory* memory);
void put_reg(Cpu *cpu, Register reg, uint16_t val);
void stack_push_16b(Cpu *cpu, Memory *memory, uint16_t val);
uint16_t stack_pop_16b(Cpu *cpu, Memory *memory);
void set_flag(Cpu *cpu, Flag flag, State state);
State read_flag(Cpu *cpu, Flag flag);
State check_for_zero(uint16_t val);
State check_for_carry(Cpu *cpu, uint16_t val_1, uint16_t val_2, Operation op);
State check_for_half_carry(Cpu *cpu, uint16_t val_1, uint16_t val_2, Operation op);
uint8_t check_condition(Cpu* cpu, Condition cond);
int64_t cpu_step(Cpu *cpu, Memory *memory);

