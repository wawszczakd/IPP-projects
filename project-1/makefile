CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2

all: labyrinth

labyrinth: labyrinth.o vector.o bitset.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

dist: clean
	zip labyrinth.zip labyrinth.c vector.h vector.c bitset.h bitset.c queue.h queue.c makefile test.sh

clean:
	-rm -f *.o labyrinth labyrinth.zip

.PHONY: all clean dist
