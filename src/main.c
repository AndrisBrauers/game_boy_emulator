#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    printf("Loading ROM: %s\n", argv[1]);

    FILE * file = fopen(argv[1], "rb");

    if (file == NULL)
    {
        perror("Could not open the ROM file.");
        return 1;
    }

    if (fseek(file, 0, SEEK_END) != 0) perror("fseek failed.");

    int64_t rom_size = ftell(file);
    rewind(file);

    if (rom_size == -1L)
    {
        perror("Error occored while reading ROM file.");
        return 1;
    }

    if (rom_size == 0)
    {
        perror("ROM file 0 byte long.");
        return 1;
    }

    printf("ROM size: %lli bytes\n", rom_size);
   
    uint8_t *rom_ptr = malloc(rom_size);
    if (rom_ptr == NULL)
    {
        perror("Did not manage to allocate memory for ROM.");
        return 1;
    }

    size_t r = fread(rom_ptr, 1, rom_size, file);
    if (r != (uint64_t)rom_size)
    {
        printf("ROM file read incorectly.\n");
        return 1;
    };

    fclose(file);

    return 0;
}