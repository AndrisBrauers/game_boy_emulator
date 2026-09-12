#pragma once
#include "stdint.h"
#include "cpu.h"
#include "bus.h"


typedef struct Instruction Instruction;

typedef int16_t (*InstructionHandler)(
    Cpu *cpu,
    Memory *memory,
    const Instruction *instruction
);

struct Instruction {
    InstructionHandler handler;
    uint8_t opcode;
    Register dest;
    Register source;
    Condition condition;
    uint8_t bit_number;
    int16_t cycles;
    int16_t extras_cycles;
    char name[20];
};

Instruction decode_base_opcode(uint8_t opcode);
Instruction decode_cb_opcode(uint8_t opcode);

Instruction set_instruction(InstructionHandler handler, uint8_t opcode, 
                            Register dest, Register source, Condition condition, uint8_t bit_number, 
                            int16_t cycles, int16_t extra_cycles, char* name);
void init_decoder_tables();
