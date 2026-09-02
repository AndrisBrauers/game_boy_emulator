#include "stdio.h"
#include "cartridge.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    Cartridge game_cartridge;
    game_cartridge.name = argv[1];
    load_rom(&game_cartridge);

    return 0;
}