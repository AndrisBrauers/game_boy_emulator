#include "stdio.h"
#include "cartridge.h"
#include "cpu.h"

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
    initilize_cpu(&cpu);
    print_cpu(&cpu);

    printf("\n AF: %.4x \n", get_16b_register(&cpu, 8));

    /* unload_cartridge(&game_cartridge); */

    return 0;
}