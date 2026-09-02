CC = clang

CFLAGS = -Wall -Wextra -Werror -std=c11 -g

gameboy: src/main.c
	$(CC) $(CFLAGS) src/main.c -o gameboy

clean:
	rm -f gameboy