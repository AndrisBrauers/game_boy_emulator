#include "stdio.h"
#include "stdint.h"
#include "cpu.h"
#include "bus.h"
#include "decoder.h"

Register register_decoding(uint8_t reg_encoding)
{
    uint8_t reg_bits = reg_encoding & 0b00000111;
    return (reg_bits == 0b000) ? REG_B : 
           (reg_bits == 0b001) ? REG_C : 
           (reg_bits == 0b010) ? REG_D : 
           (reg_bits == 0b011) ? REG_E :
           (reg_bits == 0b100) ? REG_H : 
           (reg_bits == 0b101) ? REG_L : 
           (reg_bits == 0b110) ? REG_HL : 
                                 REG_A;
}

int16_t instruction_invalid(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)cpu;
    (void)memory;
    printf("Invalid instruction\n");
    return instruction->cycles;
}
int16_t instruction_nop(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)cpu;
    (void)memory;
    return instruction->cycles;
}
int16_t instruction_stop(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    fetch_8b(cpu, memory);
    return instruction->cycles;
}
int16_t instruction_halt(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)cpu;
    (void)memory;
    return instruction->cycles;
}
int16_t instruction_di(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    cpu->IME = OFF;
    return instruction->cycles;
}
int16_t instruction_ei(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)cpu;
    (void)memory;
    return instruction->cycles;
}
int16_t instruction_ld(Cpu *cpu, Memory *memory, const Instruction *instruction)
{

    Register dest_reg = instruction->dest;
    Register source_reg = instruction->source;
    uint8_t opcode = instruction->opcode;
    uint8_t opcode_up = (opcode & 0xF0) >> 4;
    uint8_t opcode_low = opcode & 0x0F;

    /* LD rr n16 */
    if (opcode_up >= 0x00 && opcode_up <= 0x03 && opcode_low == 0x01)
    {
        uint16_t val = fetch_16b(cpu, memory);
        put_reg(cpu, dest_reg, val);
    }
    /* LD [BC] A */
    else if (opcode == 0x02)
    {
        put_8b_val(get_16b_register(cpu, REG_BC), memory, get_8b_register(cpu, REG_A));
    }
    /* LD [DE] A */
    else if (opcode == 0x12)
    {
        put_8b_val(get_16b_register(cpu, REG_DE), memory, get_8b_register(cpu, REG_A));
    }
    /* LD [HL+] A */
    else if (opcode == 0x22)
    {
        put_8b_val(get_16b_register(cpu, REG_HL), memory, get_8b_register(cpu, REG_A));
        uint16_t new_hl = (get_16b_register(cpu, REG_HL) + 1);
        put_reg(cpu, REG_HL, new_hl);
    }
    /* LD [HL-] A */
    else if (opcode == 0x32)
    {
        put_8b_val(get_16b_register(cpu, REG_HL), memory, get_8b_register(cpu, REG_A));
        uint16_t new_hl = (get_16b_register(cpu, REG_HL) - 1);
        put_reg(cpu, REG_HL, new_hl);
    }
    /* LD B n8 */
    else if (opcode_up >= 0x00 && opcode_up <= 0x02 && opcode_low == 0x06)
    {
        uint8_t val = fetch_8b(cpu, memory);
        put_reg(cpu, dest_reg, val);
    }
    /* LD [HL] n8 */
    else if (opcode == 0x36)
    {
        uint8_t val = fetch_8b(cpu, memory);
        uint16_t hl_addr = get_16b_register(cpu, REG_HL);
        put_8b_val(hl_addr, memory, val);
    }
    /* LD [a16] SP */
    else if (opcode == 0x08)
    {
        uint16_t val = get_16b_register(cpu, REG_SP);
        uint16_t dest_addr = fetch_16b(cpu,memory);
        put_16b_val(dest_addr, memory, val);
    }
        /* LD A [BC] */
    else if (opcode == 0x0A)
    {
        put_reg(cpu, REG_A, get_8b_val(get_16b_register(cpu, REG_BC), memory));
    }
    /* LD A (DE) */
    else if (opcode == 0x1A)
    {
        put_reg(cpu, REG_A, get_8b_val(get_16b_register(cpu, REG_DE), memory));
    }
    /* LD A (HL+) */
    else if (opcode == 0x2A)
    {
        put_reg(cpu, REG_A, get_8b_val(get_16b_register(cpu, REG_HL), memory));
        uint16_t new_hl = (get_16b_register(cpu, REG_HL) + 1);
        put_reg(cpu, REG_HL, new_hl);
    }
    /* LD A (HL-) */
    else if (opcode == 0x3A)
    {
        put_reg(cpu, REG_A, get_8b_val(get_16b_register(cpu, REG_HL), memory));
        uint16_t new_hl = (get_16b_register(cpu, REG_HL) - 1);
        put_reg(cpu, REG_HL, new_hl);
    }
    /* LD C n8 */
    else if (opcode_up >= 0x00 && opcode_up <= 0x03 && opcode_low == 0x0E)
    {
        uint8_t val = fetch_8b(cpu, memory);
        put_reg(cpu, dest_reg, val);
    }
    /* LD r r */
    else if (opcode >= 0x40 && opcode <= 0x7F)
    {
        if (dest_reg == MEM_HL)
        {
            put_8b_val(get_16b_register(cpu, REG_HL), memory, get_8b_register(cpu, source_reg));
        }
        else if (source_reg == MEM_HL)
        {
            put_reg(cpu, dest_reg, get_8b_val(get_16b_register(cpu, REG_HL), memory));
        } else 
        {
            put_reg(cpu, dest_reg, get_8b_register(cpu, source_reg));
        }
    }
    /* LDH [a8] A */
    else if (opcode == 0xE0)
    {
        uint16_t addr = (fetch_8b(cpu, memory) + 0xFF00);
        put_8b_val(addr, memory, get_8b_register(cpu, REG_A));
    }
    /* LDH A [a8] */
    else if (opcode == 0xF0)
    {
        uint8_t val = get_8b_val((fetch_8b(cpu, memory) + 0xFF00), memory);
        put_reg(cpu, REG_A, val);
    }
    /* LDH [C] A */
    else if (opcode == 0xE2)
    {
        uint16_t addr = get_8b_register(cpu, REG_C) + 0xFF00;
        put_8b_val(addr, memory, get_8b_register(cpu, REG_A));
    }
    /* LDH A [C] */
    else if (opcode == 0xF2)
    {
        uint8_t val = get_8b_val((get_8b_register(cpu, REG_C) + 0xFF00), memory);
        put_reg(cpu, REG_A, val);
    }
    /* LD HL SP+e8 */
    else if (opcode == 0xF8) 
    {
        int8_t offset = fetch_8b(cpu, memory);
        uint16_t sp_val = get_16b_register(cpu, REG_SP);
        put_reg(cpu, REG_HL, (sp_val + offset));

        set_flag(cpu, FLAG_N, OFF);
        set_flag(cpu, FLAG_Z, OFF);

        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, (sp_val & 0x00FF), (uint8_t)offset, ADD));
        set_flag(cpu, FLAG_C, check_for_carry(cpu, (sp_val & 0x00FF), (uint8_t)offset, ADD));
    }
    /* LD SP HL */
    else if (opcode == 0xF9)
    {
        put_reg(cpu, REG_SP, get_16b_register(cpu, REG_HL));
    }
    /* LD [a16] A */
    else if (opcode == 0xEA)
    {
        uint16_t dest_addr = fetch_16b(cpu,memory);
        put_8b_val(dest_addr, memory, get_8b_register(cpu, REG_A));
    }
    /* LD A [a16] */
    else if (opcode == 0xFA)
    {
        uint16_t source_addr = fetch_16b(cpu,memory);
        put_reg(cpu, REG_A, get_8b_val(source_addr, memory));
    }

    return instruction->cycles;
}
int16_t instruction_add(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register dest_reg = instruction->dest;
    Register source_reg = instruction->source;
    uint8_t opcode = instruction->opcode;
    uint8_t opcode_up = (opcode & 0xF0) >> 4;
    uint8_t opcode_low = opcode & 0x0F;
    /* ADD HL rr */
    if (opcode_up >= 0x00 && opcode_up <= 0x03 && opcode_low == 0x09)
    {
        uint16_t first_val = get_16b_register(cpu, dest_reg);
        uint16_t second_val = get_16b_register(cpu, source_reg);

        uint16_t res = first_val + second_val;

        set_flag(cpu, FLAG_N, OFF);
        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, first_val, second_val, ADD_16B));
        set_flag(cpu, FLAG_C, check_for_carry(cpu, first_val, second_val, ADD_16B));

        put_reg(cpu, dest_reg, res);
    }
    /* ADD A r */
    else if (opcode >= 0x80 && opcode <= 0x87)
    {
        uint8_t reg_A_val = get_8b_register(cpu, REG_A);
        uint8_t second_val = 0;

        if (source_reg == MEM_HL)
        {
            uint16_t hl_addr = get_16b_register(cpu, REG_HL);
            second_val = get_8b_val(hl_addr, memory);
        } 
        else 
        {
            second_val = get_8b_register(cpu, source_reg);
        }

        uint8_t sum = reg_A_val + second_val;

        set_flag(cpu, FLAG_Z, check_for_zero(sum));
        set_flag(cpu, FLAG_N, OFF);
        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, reg_A_val, second_val, ADD));
        set_flag(cpu, FLAG_C, check_for_carry(cpu, reg_A_val, second_val, ADD));

        put_reg(cpu, REG_A, sum);
    }
    /* ADD A n8 */
    else if (opcode == 0xC6) 
    {
        uint8_t reg_A_val = get_8b_register(cpu, REG_A);
        uint8_t second_val = fetch_8b(cpu, memory);

        uint8_t res = reg_A_val + second_val;

        set_flag(cpu, FLAG_Z, check_for_zero(res));
        set_flag(cpu, FLAG_N, OFF);
        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, reg_A_val, second_val, ADD));
        set_flag(cpu, FLAG_C, check_for_carry(cpu, reg_A_val, second_val, ADD));

        put_reg(cpu, REG_A, res);
    }
    /* ADD SP e8 */
    else if (opcode == 0xE8) 
    {
        int8_t offset = fetch_8b(cpu, memory);
        uint16_t sp_val = get_16b_register(cpu, REG_SP);
        put_reg(cpu, REG_SP, (sp_val + offset));

        set_flag(cpu, FLAG_N, OFF);
        set_flag(cpu, FLAG_Z, OFF);

        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, (sp_val & 0x00FF), (uint8_t)offset, ADD));
        set_flag(cpu, FLAG_C, check_for_carry(cpu, (sp_val & 0x00FF), (uint8_t)offset, ADD));
    }

    return instruction->cycles;
}
int16_t instruction_adc(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t carry_val = read_flag(cpu, FLAG_C);
    uint8_t sum = reg_A_val + second_val + carry_val;

    set_flag(cpu, FLAG_Z, check_for_zero(sum));
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, check_for_half_carry(cpu, reg_A_val, second_val, ADC));
    set_flag(cpu, FLAG_C, check_for_carry(cpu, reg_A_val, second_val, ADC));

    put_reg(cpu, REG_A, sum);

    return instruction->cycles;
}
int16_t instruction_inc(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register dest_reg = instruction->dest;
    uint8_t opcode = instruction->opcode;
    uint8_t opcode_low = opcode & 0x0F;

    if (opcode_low != 0x03)
    {
        uint8_t val = (dest_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) :  get_8b_register(cpu, dest_reg);
        val++;

        set_flag(cpu, FLAG_Z, check_for_zero(val));
        set_flag(cpu, FLAG_N, OFF);
        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, (val - 1), 1, INC));

        if (dest_reg == MEM_HL) 
            put_8b_val(get_16b_register(cpu, REG_HL), memory, val);
        else
            put_reg(cpu, dest_reg, val);
    }
    else
    {
        uint16_t val = get_16b_register(cpu, dest_reg);
        val++;
        put_reg(cpu, dest_reg, val);
    }
    return instruction->cycles;
}
int16_t instruction_sub(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t res = reg_A_val - second_val;

    set_flag(cpu, FLAG_Z, check_for_zero(res));
    set_flag(cpu, FLAG_N, ON);
    set_flag(cpu, FLAG_H, check_for_half_carry(cpu, reg_A_val, second_val, SUB));
    set_flag(cpu, FLAG_C, check_for_carry(cpu, reg_A_val, second_val, SUB));

    put_reg(cpu, REG_A, res);

    return instruction->cycles;
}
int16_t instruction_sbc(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t res = reg_A_val - second_val - read_flag(cpu, FLAG_C);

    set_flag(cpu, FLAG_Z, check_for_zero(res));
    set_flag(cpu, FLAG_N, ON);
    set_flag(cpu, FLAG_H, check_for_half_carry(cpu, reg_A_val, second_val, SBC));
    set_flag(cpu, FLAG_C, check_for_carry(cpu, reg_A_val, second_val, SBC));

    put_reg(cpu, REG_A, res);

    return instruction->cycles;
}
int16_t instruction_dec(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register dest_reg = instruction->dest;
    uint8_t opcode = instruction->opcode;
    uint8_t opcode_low = opcode & 0x0F;

    if (opcode_low != 0x0B)
    {
        uint8_t val = (dest_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) :  get_8b_val(dest_reg, memory);
        val--;

        set_flag(cpu, FLAG_Z, check_for_zero(val));
        set_flag(cpu, FLAG_N, ON);
        set_flag(cpu, FLAG_H, check_for_half_carry(cpu, (val + 1), 1, DEC));

        if (dest_reg == MEM_HL) 
            put_8b_val(get_16b_register(cpu, REG_HL), memory, val);
        else
            put_reg(cpu, dest_reg, val);
    }
    else
    {
        uint16_t val = get_16b_register(cpu, dest_reg);
        val--;
        put_reg(cpu, dest_reg, val);
    }
    return instruction->cycles;
}
int16_t instruction_and(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t res = reg_A_val & second_val;

    set_flag(cpu, FLAG_Z, check_for_zero(res));
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, ON);
    set_flag(cpu, FLAG_C, OFF);

    put_reg(cpu, REG_A, res);

    return instruction->cycles;
}
int16_t instruction_or(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t res = reg_A_val | second_val;

    set_flag(cpu, FLAG_Z, check_for_zero(res));
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, OFF);
    set_flag(cpu, FLAG_C, OFF);

    put_reg(cpu, REG_A, res);

    return instruction->cycles;
}
int16_t instruction_xor(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t res = reg_A_val ^ second_val;

    set_flag(cpu, FLAG_Z, check_for_zero(res));
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, OFF);
    set_flag(cpu, FLAG_C, OFF);

    put_reg(cpu, REG_A, res);

    return instruction->cycles;
}
int16_t instruction_cp(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    Register source_reg = instruction->source;

    uint8_t reg_A_val = get_8b_register(cpu, REG_A);
    uint8_t second_val = (source_reg == MEM_HL) ? get_8b_val(get_16b_register(cpu, REG_HL), memory) : 
                         (source_reg == IMM8) ? fetch_8b(cpu, memory) : get_8b_register(cpu, source_reg);

    uint8_t res = reg_A_val - second_val;

    set_flag(cpu, FLAG_Z, check_for_zero(res));
    set_flag(cpu, FLAG_N, ON);
    set_flag(cpu, FLAG_H, check_for_half_carry(cpu, reg_A_val, second_val, SUB));
    set_flag(cpu, FLAG_C, check_for_carry(cpu, reg_A_val, second_val, SUB));

    return instruction->cycles;
}
int16_t instruction_rotate(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    uint8_t opcode = instruction->opcode;
    uint8_t val = get_8b_register(cpu, REG_A);

        /* RLCA */
    if (opcode == 0x07)
    {
        State old_bit_7 = check_for_carry(cpu, val, val, RLCA);
        set_flag(cpu, FLAG_C, old_bit_7);
        val <<= 1;
        if (old_bit_7 == ON) val |= 0x01;
    }
    /* RLA */
    else if (opcode == 0x17)
    {
        State old_c = read_flag(cpu, FLAG_C);
        State new_c = check_for_carry(cpu, val, val, RLA);
        set_flag(cpu, FLAG_C, new_c);
        val <<= 1;
        if (old_c == ON) val |= 0x01;
    }
    /* RRCA */
    else if (opcode == 0x0F)
    {
        State old_bit_0 = check_for_carry(cpu, val, val, RRCA);
        set_flag(cpu, FLAG_C, old_bit_0);
        val >>= 1;
        if (old_bit_0 == ON) val |= 0x80;
    }
    /* RRA */
    else if (opcode == 0x1F)
    {
        State old_c = read_flag(cpu, FLAG_C);
        State new_c = check_for_carry(cpu, val, val, RRA);
        set_flag(cpu, FLAG_C, new_c);
        val >>= 1;
        if (old_c == ON) val |= 0x80;
    }

    set_flag(cpu, FLAG_Z, OFF);
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, OFF);

    put_reg(cpu, REG_A, val);
    return instruction->cycles;
}
int16_t instruction_pop(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    put_reg(cpu, instruction->dest, stack_pop_16b(cpu, memory));
    return instruction->cycles;
}
int16_t instruction_push(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    stack_push_16b(cpu, memory, get_16b_register(cpu, instruction->dest));
    return instruction->cycles;
}

int16_t instruction_ret(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    if (check_condition(cpu, instruction->condition) == 1)
    {
        uint16_t new_pc = stack_pop_16b(cpu, memory);
        cpu->PC = new_pc;
        if (instruction->condition != NO_COND)
            return instruction->extras_cycles;
    }
    return instruction->cycles;
}
int16_t instruction_jp(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    uint16_t jp_address = (instruction->dest == REG_HL) ? get_16b_register(cpu, REG_HL) : fetch_16b(cpu, memory);
    if (check_condition(cpu, instruction->condition) == 1)
    {
        cpu->PC = jp_address;
        if (instruction->condition != NO_COND)
            return instruction->extras_cycles;
    }

    return instruction->cycles;
}

int16_t instruction_jr(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    int8_t jp_offset = (int8_t)fetch_8b(cpu, memory);

    if (check_condition(cpu, instruction->condition) == 1)
    {
        cpu->PC += jp_offset;
        if (instruction->condition != NO_COND)
            return instruction->extras_cycles;
    }
    return instruction->cycles;
}
int16_t instruction_call(Cpu *cpu, Memory *memory, const Instruction *instruction)
{

    uint16_t new_pc = fetch_16b(cpu, memory);

    if (check_condition(cpu, instruction->condition) == 1)
    {
        stack_push_16b(cpu, memory, cpu->PC);
        cpu->PC = new_pc;
        if (instruction->condition != NO_COND)
            return instruction->extras_cycles;
    }
    return instruction->cycles;
}
int16_t instruction_rst(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    stack_push_16b(cpu, memory, cpu->PC);
    cpu->PC = instruction->bit_number;
    return instruction->cycles;
}
int16_t instruction_reti(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    cpu->PC = stack_pop_16b(cpu, memory);
    cpu->IME = ON;
    return instruction->cycles;
}

int16_t instruction_daa(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    uint8_t val = get_8b_register(cpu, REG_A);
    uint8_t correction = 0;
    if (read_flag(cpu, FLAG_N) == OFF)
    {
        if (((val & 0x0F) > 9) || (read_flag(cpu, FLAG_H) == ON)) correction |= 0x06;
        if ((val > 0x99) || (read_flag(cpu, FLAG_C) == ON)) 
        {
            correction |= 0x60;
            set_flag(cpu, FLAG_C, ON);
        }

        val += correction;
    }
    else
    {
        if ((read_flag(cpu, FLAG_H) == ON)) correction |= 0x06;
        if ((read_flag(cpu, FLAG_C) == ON)) correction |= 0x60;

        val -= correction;
    }

    put_reg(cpu, REG_A, val);
    set_flag(cpu, FLAG_Z, check_for_zero(val));
    set_flag(cpu, FLAG_H, OFF);

    return instruction->cycles;
}
int16_t instruction_scf(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, OFF);
    set_flag(cpu, FLAG_C, ON);
    return instruction->cycles;
}
int16_t instruction_cpl(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    uint8_t val = get_8b_register(cpu, REG_A);
    val = ~val;
    set_flag(cpu, FLAG_N, ON);
    set_flag(cpu, FLAG_H, ON);
    put_reg(cpu, REG_A, val);
    return instruction->cycles;
}
int16_t instruction_ccf(Cpu *cpu, Memory *memory, const Instruction *instruction)
{
    (void)memory;
    State val = read_flag(cpu, FLAG_C);
    val = (val == ON) ? OFF : ON;
    set_flag(cpu, FLAG_N, OFF);
    set_flag(cpu, FLAG_H, OFF);
    set_flag(cpu, FLAG_C, val);
    return instruction->cycles;
}
/*
int16_t instruction_rlc(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_rrc(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_rl(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_rr(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_sla(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_sra(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_swap(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_srl(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_bit(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_res(Cpu *cpu, Memory *memory, const Instruction *instruction);
int16_t instruction_set(Cpu *cpu, Memory *memory, const Instruction *instruction);
*/
