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

void set_flag(Cpu *cpu, Flag flag, Flag_state state)
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

uint8_t read_flag(Cpu *cpu, Flag flag)
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
    return (cpu->F & mask);
}

int64_t cpu_step(Cpu *cpu, Cartridge *cartridge)
{
    uint16_t pc_address = get_16b_register(cpu, REG_PC);
    uint8_t opcode = get_address(pc_address, cartridge);
    int32_t cycles = 0;

    cpu->PC++;

    if (opcode == 0x00)
    {
        cycles = 4;
    }
    else if (opcode == 0x01)
    {
        uint16_t val = fetch_16b(cpu, cartridge);
        put_reg(cpu, REG_BC, val);
        cycles = 12;
    }
    else if (opcode == 0x06)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_B, val);
        cycles = 8;
    }
    else if (opcode == 0x0E)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_C, val);
        cycles = 8;
    }
    else if (opcode == 0x11)
    {
        uint16_t val = fetch_16b(cpu, cartridge);
        put_reg(cpu, REG_DE, val);
        cycles = 12;
    }
    else if (opcode == 0x16)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_D, val);
        cycles = 8;
    }
    else if (opcode == 0x1E)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_E, val);
        cycles = 8;
    }
    else if (opcode == 0x21)
    {
        uint16_t val = fetch_16b(cpu, cartridge);
        put_reg(cpu, REG_HL, val);
        cycles = 12;
    }
    else if (opcode == 0x26)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_H, val);
        cycles = 8;
    }
    else if (opcode == 0x2E)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_L, val);
        cycles = 8;
    }
    else if (opcode == 0x31)
    {
        uint16_t val = fetch_16b(cpu, cartridge);
        put_reg(cpu, REG_SP, val);
        cycles = 12;
    }
    else if (opcode == 0x3E)
    {
        uint8_t val = fetch_8b(cpu, cartridge);
        put_reg(cpu, REG_A, val);
        cycles = 8;
    }
    else if (opcode == 0xC3)
    {
        uint16_t jp_address = fetch_16b(cpu, cartridge);
        cpu->PC = jp_address;
        cycles = 16;
    }
    else if (opcode >= 0x40 && opcode <= 0x7F)
    {
        uint8_t dest_bits = ((opcode >> 3) & 0b00000111) ;
        uint8_t source_bits = opcode & 0b00000111;

        Register dest_reg = (dest_bits == 0b000) ? REG_B : (dest_bits == 0b001) ? REG_C : (dest_bits == 0b010) ? REG_D : (dest_bits == 0b011) ? REG_E :
                            (dest_bits == 0b100) ? REG_H : (dest_bits == 0b101) ? REG_L : (dest_bits == 0b110) ? REG_HL : REG_A;
        Register source_reg = (source_bits == 0b000) ? REG_B : (source_bits == 0b001) ? REG_C : (source_bits == 0b010) ? REG_D : (source_bits == 0b011) ? REG_E :
                            (source_bits == 0b100) ? REG_H : (source_bits == 0b101) ? REG_L : (source_bits == 0b110) ? REG_HL : REG_A;
        
        if (dest_reg == REG_HL)
        {
            if (source_reg == REG_HL)
            {
                printf("HALTING PROGRAM");
                cycles = 4;
            } else 
            {
                put_address(get_address(get_8b_register(cpu, dest_reg), cartridge), cartridge, get_8b_register(cpu, source_reg));
                cycles = 8;
            }
        }
        else if (source_reg == REG_HL)
        {
            put_reg(cpu, dest_reg, get_address(get_8b_register(cpu, source_reg), cartridge));
            cycles = 8;
        } else {
            put_reg(cpu, dest_reg, get_8b_register(cpu, source_reg));
            cycles = 4;
        }
    }
    else if (opcode == 0x04)
    {
        ((cpu->B & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->B++;
        
        (cpu->B == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x14)
    {
        ((cpu->D & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->D++;
        
        (cpu->D == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x24)
    {
        ((cpu->H & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->H++;
        
        (cpu->H == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x0C)
    {
        ((cpu->C & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->C++;
        
        (cpu->C == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x1C)
    {
        ((cpu->E & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->E++;
        
        (cpu->E == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x2C)
    {
        ((cpu->L & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->L++;
        
        (cpu->L == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x3C)
    {
        ((cpu->A & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        cpu->A++;
        
        (cpu->A == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x05)
    {
        ((cpu->B & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->B++;
        
        (cpu->B == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x15)
    {
        ((cpu->D & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->D++;
        
        (cpu->D == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x25)
    {
        ((cpu->H & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->H++;
        
        (cpu->H == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x0D)
    {
        ((cpu->C & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->C++;
        
        (cpu->C == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x1D)
    {
        ((cpu->E & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->E++;
        
        (cpu->E == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x2D)
    {
        ((cpu->L & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->L++;
        
        (cpu->L == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x3D)
    {
        ((cpu->A & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        cpu->A++;
        
        (cpu->A == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        cycles = 4;
    }
    else if (opcode == 0x34)
    {
        uint16_t hl_addr = get_16b_register(cpu, REG_HL);
        uint8_t hl_addr_val = get_address(hl_addr, cartridge);

        ((hl_addr_val & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, OFF);

        hl_addr_val++;
        
        (hl_addr_val == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        put_address(hl_addr, cartridge, hl_addr_val);
        cycles = 4;
    }
        else if (opcode == 0x35)
    {
        uint16_t hl_addr = get_16b_register(cpu, REG_HL);
        uint8_t hl_addr_val = get_address(hl_addr, cartridge);

        ((hl_addr_val & 0x0F) == 0x0F) ? set_flag(cpu, FLAG_H, ON) : set_flag(cpu, FLAG_H, OFF);
        set_flag(cpu, FLAG_N, ON);

        hl_addr_val--;
        
        (hl_addr_val == 0) ? set_flag(cpu, FLAG_Z, ON) : set_flag(cpu, FLAG_Z, OFF);
        
        put_address(hl_addr, cartridge, hl_addr_val);
        cycles = 4;
    }
    
    else
    {
        printf("Unsupported opcode\n");
        return -1;
    }

    printf("PC=%.4x OPCODE=%.2x cycles=%i \n", pc_address, opcode, cycles);
    return cycles;
}
