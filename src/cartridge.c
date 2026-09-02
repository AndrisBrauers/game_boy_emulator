#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "cartridge.h"

#define TITLE_ADDRESS 0x0134
#define TITLE_LENGTH 15
#define TYPE_ADDRESS 0x0147
#define ROM_SIZE_ADDRESS 0x0148
#define RAM_SIZE_ADDRESS 0x0149
#define HEADER_CHECKSUM_ADDRESS 0x014D


int load_cartridge(Cartridge *cartridge)
{
    FILE * file = fopen(cartridge->path, "rb");

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
    cartridge->read_size = ftell(file);
    rewind(file);

    if (cartridge->read_size == -1L)
    {
        perror("Error occored while reading ROM file.");
        fclose(file);
        return 1;
    }

    if (cartridge->read_size < 0x150){
        printf("ROM file shorter that 0x150 bytes. Invalid ROM file.");
        fclose(file);
        return 1;
    }
   
    cartridge->rom_ptr = malloc(cartridge->read_size);
    if (cartridge->rom_ptr == NULL)
    {
        perror("Did not manage to allocate memory for ROM.");
        fclose(file);
        free(cartridge->rom_ptr);
        return 1;
    }

    size_t r = fread(cartridge->rom_ptr, 1, cartridge->read_size, file);
    if (r != (uint64_t)cartridge->read_size)
    {
        printf("ROM file read incorectly.\n");
        fclose(file);
        free(cartridge->rom_ptr);
        return 1;
    };
    fclose(file);

    memcpy(cartridge->title, (cartridge->rom_ptr) + TITLE_ADDRESS, TITLE_LENGTH);
    cartridge->title[TITLE_LENGTH] = '\0';

    cartridge->type = *((cartridge->rom_ptr) + TYPE_ADDRESS);
    cartridge->rom_size_byte = *((cartridge->rom_ptr) + ROM_SIZE_ADDRESS);
    cartridge->ram_size_byte = *((cartridge->rom_ptr) + RAM_SIZE_ADDRESS);

    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - cartridge->rom_ptr[address] - 1;
    }

    if(checksum != cartridge->rom_ptr[HEADER_CHECKSUM_ADDRESS])
    {
        printf("Cartridge checksum does not match\n");
        return 1;
    }

    return 0;
}

int unload_cartridge(Cartridge *cartridge)
{
    free(cartridge->rom_ptr);
    return 0;
}

void print_cartridge(Cartridge *cartridge)
{
    printf("Cartridge path: %s \n", cartridge->path);
    printf("Cartridge title: %s \n", cartridge->title);
    printf("Cartridge type: 0x%.2x \n", cartridge->type);
    printf("Cartridge size: %lli bytes \n", cartridge->read_size);
    printf("Cartridge size from rom_size: %i bytes \n", 32768 * (1 << cartridge->rom_size_byte));
    printf("Cartridge ram size: 0x%.2x \n", cartridge->ram_size_byte);
}