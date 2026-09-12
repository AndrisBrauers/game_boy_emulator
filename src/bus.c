#include "bus.h"
#include "stdint.h"
#include "cartridge.h"
#include "stdio.h"
#include "stdlib.h"

#define RAM_SIZE 8192
#define HRAM_SIZE 127

void initilize_memory(Memory* memory, Cartridge* cartridge)
{
    memory->rom = cartridge->rom_ptr;
    memory->ram = malloc(RAM_SIZE);
    memory->hram = malloc(HRAM_SIZE);
}

void unload_memory(Memory* memory)
{
    free(memory->ram);
}

uint8_t get_8b_val(uint16_t address, Memory* memory)
{
    if (address >= 0x0000 & address < 0xC000)
    {
        return memory->rom[address];
    } 
    else if (address >= 0xC000 & address < 0xE000)
    {
        return memory->ram[address];
    } 
    else if (address >= 0xFF80 & address < 0xFFFF)
    {
        return memory->hram[address];
    } 
 
    printf("Address 0x%.2x out of bounds of memory", address);
    
    return 0;
}

void put_8b_val(uint16_t address, Memory* memory, uint8_t val)
{
    if (address >= 0x0000 & address < 0xC000)
    {
         printf("Address 0x%.2x can not be put in ROM", address);
    } 
    else if (address >= 0xC000 & address < 0xE000)
    {
        memory->ram[address] = val;
    } 
    else if (address >= 0xFF80 & address < 0xFFFF)
    {
        memory->hram[address] = val;
    } 
}

void put_16b_val(uint16_t address, Memory* memory, uint16_t val)
{
    uint8_t byte_1 = (uint8_t)((val & 0xFF00) >> 8);
    uint8_t byte_2 = (uint8_t)(val & 0x00FF);

    put_8b_val(address, memory, byte_2);
    put_8b_val((address + 1), memory, byte_1);
}

