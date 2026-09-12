#pragma once
#include "stdint.h"
#include "cpu.h"
#include "bus.h"
#include "decoder.h"

Register register_decoding(uint8_t reg_encoding);
int16_t instruction_invalid(Cpu *, Memory *, const Instruction *);
int16_t instruction_nop(Cpu *, Memory *, const Instruction *);
int16_t instruction_stop(Cpu *, Memory *, const Instruction *);
int16_t instruction_halt(Cpu *, Memory *, const Instruction *);
int16_t instruction_di(Cpu *, Memory *, const Instruction *);
int16_t instruction_ei(Cpu *, Memory *, const Instruction *);

int16_t instruction_ld(Cpu *, Memory *, const Instruction *);

int16_t instruction_add(Cpu *, Memory *, const Instruction *);
int16_t instruction_adc(Cpu *, Memory *, const Instruction *);
int16_t instruction_inc(Cpu *, Memory *, const Instruction *);
int16_t instruction_sub(Cpu *, Memory *, const Instruction *);
int16_t instruction_sbc(Cpu *, Memory *, const Instruction *);
int16_t instruction_dec(Cpu *, Memory *, const Instruction *);
int16_t instruction_and(Cpu *, Memory *, const Instruction *);
int16_t instruction_or(Cpu *, Memory *, const Instruction *);
int16_t instruction_xor(Cpu *, Memory *, const Instruction *);
int16_t instruction_cp(Cpu *, Memory *, const Instruction *);
int16_t instruction_rotate(Cpu *, Memory *, const Instruction *);

int16_t instruction_pop(Cpu *, Memory *, const Instruction *);
int16_t instruction_push(Cpu *, Memory *, const Instruction *);

int16_t instruction_ret(Cpu *, Memory *, const Instruction *);
int16_t instruction_jp(Cpu *, Memory *, const Instruction *);
int16_t instruction_jr(Cpu *, Memory *, const Instruction *);
int16_t instruction_call(Cpu *, Memory *, const Instruction *);
int16_t instruction_rst(Cpu *, Memory *, const Instruction *);
int16_t instruction_reti(Cpu *, Memory *, const Instruction *);

int16_t instruction_daa(Cpu *, Memory *, const Instruction *);
int16_t instruction_scf(Cpu *, Memory *, const Instruction *);
int16_t instruction_cpl(Cpu *, Memory *, const Instruction *);
int16_t instruction_ccf(Cpu *, Memory *, const Instruction *);

int16_t instruction_rlc(Cpu *, Memory *, const Instruction *);
int16_t instruction_rrc(Cpu *, Memory *, const Instruction *);
int16_t instruction_rl(Cpu *, Memory *, const Instruction *);
int16_t instruction_rr(Cpu *, Memory *, const Instruction *);
int16_t instruction_sla(Cpu *, Memory *, const Instruction *);
int16_t instruction_sra(Cpu *, Memory *, const Instruction *);
int16_t instruction_swap(Cpu *, Memory *, const Instruction *);
int16_t instruction_srl(Cpu *, Memory *, const Instruction *);
int16_t instruction_bit(Cpu *, Memory *, const Instruction *);
int16_t instruction_res(Cpu *, Memory *, const Instruction *);
int16_t instruction_set(Cpu *, Memory *, const Instruction *);
