CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -Iinclude

SRC = src/main.c src/cartridge.c src/cpu.c src/bus.c src/decoder.c src/instructions.c

gameboy: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o gameboy

clean:
	rm -f gameboy