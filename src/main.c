#include "stdio.h"
#include "cartridge.h"
#include "cpu.h"
#include "bus.h"

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

    printf("F: %.2x \n", cpu.F);
    set_flag(&cpu, FLAG_Z, ON);
    printf("F: %.2x \n", read_flag(&cpu, FLAG_Z));
    set_flag(&cpu, FLAG_Z, OFF);
    printf("F: %.2x \n", read_flag(&cpu, FLAG_Z));
    set_flag(&cpu, FLAG_H, ON);
    printf("F: %.2x \n", read_flag(&cpu, FLAG_H));
    set_flag(&cpu, FLAG_ALL, ON);
    printf("F: %.2x \n", cpu.F);


    // put_reg(&cpu, REG_BC, 0xA354);
    // put_reg(&cpu, REG_AF, 0xA354);
    // print_cpu(&cpu);

    /* unload_cartridge(&game_cartridge); */

    return 0;
}