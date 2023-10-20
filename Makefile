CC=clang-15
CFLAGS=-Wall -pedantic -std=c99 -lc

main: main.c
	$(CC) $(CFLAGS) main.c -o main