CFLAGS := -Wall -Wextra -pedantic -std=c99 -g -O2
CC := gcc

all: elf2bin

clean:
	$(RM) elf2bin.exe elf2bin

.PHONY: all clean

elf2bin: elf2bin.c
	$(CC) $(CFLAGS) $^ -o $@