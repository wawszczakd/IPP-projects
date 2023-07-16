#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include "vector.h"

#define MAX_CAPACITY SIZE_MAX / sizeof(size_t)

bool vector_init(Vector *vector) {
	vector->items = malloc(sizeof(size_t) * 8);
	if(vector->items == NULL) {
		fprintf(stderr, "ERROR 0\n");
		return false;
	}
	vector->size = 0;
	vector->capacity = 8;
	
	return true;
}

bool vector_push_back(Vector *vector, size_t value) {
	assert(vector->capacity > 0);
	
	if(vector->size == vector->capacity) {
		if(vector->capacity == MAX_CAPACITY) {
			return false;
		}
		else if(vector->capacity > MAX_CAPACITY / 2) {
			vector->capacity = MAX_CAPACITY;
		}
		else {
			vector->capacity *= 2;
		}
		
		vector->items = realloc(vector->items, sizeof(size_t) * vector->capacity);
		if(vector->items == NULL) {
			fprintf(stderr, "ERROR 0\n");
			return false;
		}
	}
	
	assert(vector->size < vector->capacity);
	vector->items[vector->size++] = value;
	
	return true;
}

size_t vector_pop_back(Vector *vector) {
	assert(vector->size > 0);
	return vector->items[--vector->size];
}

void vector_clear(Vector *vector) {
	vector->size = vector->capacity = 0;
	free(vector->items);
	vector->items = NULL;
}
