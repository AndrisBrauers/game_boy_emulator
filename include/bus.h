#pragma once
#include "stdint.h"
#include "cartridge.h"

typedef struct Memory Memory;

struct Memory
{
    uint8_t *rom;
    uint8_t *ram;
    uint8_t *hram;
};

void initilize_memory(Memory* memory, Cartridge* cartridge);
void unload_memory(Memory* memory);
uint8_t get_8b_val(uint16_t address, Memory* memory);
void put_8b_val(uint16_t address, Memory* memory, uint8_t val);
void put_16b_val(uint16_t address, Memory* memory, uint16_t val);