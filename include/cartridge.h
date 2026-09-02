#include "stdint.h"

typedef struct
{
    char* name;
    uint8_t* rom_ptr;
    int64_t size;
} Cartridge;

int load_rom(Cartridge *c);