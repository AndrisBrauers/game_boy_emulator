#pragma once
#include "stdint.h"


typedef struct
{
    char* path;
    char title[17];
    uint8_t* rom_ptr;
    int64_t read_size;
    uint8_t type;
    uint8_t rom_size_byte;
    uint8_t ram_size_byte;
} Cartridge;

int load_cartridge(Cartridge*);
int unload_cartridge(Cartridge*);
void print_cartridge(Cartridge*);