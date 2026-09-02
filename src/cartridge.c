#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "cartridge.h"


int load_rom(Cartridge *cartridge)
{
    FILE * file = fopen(cartridge->name, "rb");

    if (file == NULL)
    {
        perror("Could not open the ROM file.");
        return 1;
    }

    if (fseek(file, 0, SEEK_END) != 0) 
    {
        perror("fseek failed.");
        fclose(file);
        return 1;
    }
    cartridge->size = ftell(file);
    rewind(file);

    if (cartridge->size == -1L)
    {
        perror("Error occored while reading ROM file.");
        fclose(file);
        return 1;
    }

    if (cartridge->size == 0)
    {
        printf("ROM file 0 byte long.");
        fclose(file);
        return 1;
    }

    printf("ROM size: %lli bytes\n", cartridge->size);
   
    cartridge->rom_ptr = malloc(cartridge->size);
    if (cartridge->rom_ptr == NULL)
    {
        perror("Did not manage to allocate memory for ROM.");
        fclose(file);
        free(cartridge->rom_ptr);
        return 1;
    }

    size_t r = fread(cartridge->rom_ptr, 1, cartridge->size, file);
    if (r != (uint64_t)cartridge->size)
    {
        printf("ROM file read incorectly.\n");
        fclose(file);
        free(cartridge->rom_ptr);
        return 1;
    };

    fclose(file);
    return 0;
}
