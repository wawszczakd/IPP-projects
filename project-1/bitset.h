#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Bitset {
	size_t size;
	uint32_t* bitarray;
} Bitset;

bool bitset_init(Bitset *, size_t);

uint32_t bitset_get(Bitset *, size_t);

void bitset_set(Bitset *, size_t, uint32_t);

void bitset_clear(Bitset *);
