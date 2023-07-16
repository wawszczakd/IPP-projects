#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Vector {
	size_t *items;
	size_t size, capacity;
} Vector;

bool vector_init(Vector *);

bool vector_push_back(Vector *, size_t);

size_t vector_pop_back(Vector *);

void vector_clear(Vector *);
