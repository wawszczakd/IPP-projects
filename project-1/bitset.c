#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include "bitset.h"

bool bitset_init(Bitset *bitset, size_t size) {
	bitset->size = size;
	bitset->bitarray = calloc(sizeof(uint32_t), (size / 32 + 1));
	
	if(bitset->bitarray == NULL) {
		fprintf(stderr, "ERROR 0\n");
		return false;
	}
	return true;
}

uint32_t bitset_get(Bitset *bitset, size_t position) {
	assert(position < bitset->size);
	
	// A cell is blocked if and only if
	// (bitset->bitarray[cell / 32] >> (cell % 32)) % 2 == 1,
	// but it is quicker to use bitwise operations.
	return (bitset->bitarray[position >> 5] >> (position & 31)) & 1;
}

void bitset_set(Bitset *bitset, size_t position, uint32_t value) {
	assert(position < bitset->size);
	assert(value <= 1);
	
	if(bitset_get(bitset, position) != value) {
		bitset->bitarray[position >> 5] ^= 1u << (position & 31);
	}
}

void bitset_clear(Bitset *bitset) {
	bitset->size = 0;
	free(bitset->bitarray);
	bitset->bitarray = NULL;
}