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