#include "bus.h"
#include "stdint.h"
#include "cartridge.h"
#include "stdio.h"

uint8_t get_address(uint16_t address, Cartridge* cartridge)
{
    if(address > cartridge->read_size){
        printf("Address 0x%.2x larger than the loaded ROM size", address);
        return 0;
    }
    if(address < 0x8000)
    {
        return cartridge->rom_ptr[address];
    }
    printf("Address 0x%.2x out of bounds of cartridge ROM", address);
    return 0;
}

void put_address(uint16_t address, Cartridge* cartridge, uint8_t val)
{
    if(address > cartridge->read_size){
        printf("Address 0x%.2x larger than the loaded ROM size", address);
    }
    cartridge->rom_ptr[address] = val;
}

uint8_t fetch_8b(Cpu *cpu,Cartridge *cartridge)
{
    uint8_t res = get_address(get_16b_register(cpu, REG_PC), cartridge);
    cpu->PC++;
    return res;
}

uint16_t fetch_16b(Cpu *cpu, Cartridge *cartridge)
{
    uint16_t res = 0;
    uint8_t byte_1 = fetch_8b(cpu, cartridge);
    uint8_t byte_2 = fetch_8b(cpu, cartridge);
    res = byte_2;
    res <<= 8;
    res |= byte_1;
    return res;
}