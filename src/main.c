#include "stdio.h"
#include "cartridge.h"
#include "cpu.h"
#include "bus.h"
#include "decoder.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    Cartridge game_cartridge;
    game_cartridge.path = argv[1];
    if (load_cartridge(&game_cartridge) == 1) return 1;
    print_cartridge(&game_cartridge);

    Cpu cpu;
    Memory memory;

    initilize_memory(&memory, &game_cartridge);
    initilize_cpu(&cpu);
    init_decoder_tables();
    print_cpu(&cpu);

    int i = 0;
    while (i < 100)
    {
        if (cpu_step(&cpu, &memory) == -1) break;
        print_cpu(&cpu);
        i++;
    }


    // put_reg(&cpu, REG_BC, 0xA354);
    // put_reg(&cpu, REG_AF, 0xA354);
    // print_cpu(&cpu);

    unload_cartridge(&game_cartridge); 
    unload_memory(&memory);

    return 0;
}