#pragma once
#include "stdint.h"
#include "cartridge.h"
#include "cpu.h"

uint8_t get_address(uint16_t address, Cartridge* cartridge);
void put_address(uint16_t address, Cartridge* cartridge, uint8_t val);
uint8_t fetch_8b(Cpu* cpu, Cartridge *cartridge);
uint16_t fetch_16b(Cpu* cpu, Cartridge *cartridge);