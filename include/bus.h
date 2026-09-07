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
uint8_t get_address(uint16_t address, Memory* memory);
void put_address(uint16_t address, Memory* memory, uint8_t val);