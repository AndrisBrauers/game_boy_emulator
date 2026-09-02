CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -Iinclude

SRC = src/main.c src/cartridge.c

gameboy: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o gameboy

clean:
	rm -f gameboy