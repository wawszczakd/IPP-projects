#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include "vector.h"
#include "bitset.h"
#include "queue.h"

// Every reading function returns true if and only if the input is correct.

bool read_first_line(Vector *dimensions, size_t *maze_size) {
	char c = getchar();
	while(c != EOF && c != '\n' && isspace(c)) {
		c = getchar();
	}
	if(c == EOF || c == '\n') {
		fprintf(stderr, "ERROR 1\n");
		return false;
	}
	
	while(true) {
		size_t n = 0; // current dimension
		while(c != EOF && !isspace(c)) {
			if(!('0' <= c && c <= '9') || n > (SIZE_MAX - (c - '0')) / 10) {
				fprintf(stderr, "ERROR 1\n");
				return false;
			}
			n = n * 10 + c - '0';
			c = getchar();
		}
		
		if(n == 0) {
			fprintf(stderr, "ERROR 1\n");
			return false;
		}
		if(*maze_size > SIZE_MAX / n) {
			fprintf(stderr, "ERROR 0\n");
			return false;
		}
		*maze_size *= n;
		if(!vector_push_back(dimensions, n)) {
			return false;
		}
		
		while(c != EOF && isspace(c)) {
			if(c == '\n') {
				break;
			}
			c = getchar();
		}
		if(c == EOF) {
			fprintf(stderr, "ERROR 2\n");
			return false;
		}
		else if(c == '\n') {
			break;
		}
	}
	
	assert(dimensions->size > 0);
	assert(c == '\n');
	
	return true;
}

bool read_second_line(size_t *start, Vector dimensions, size_t maze_size, size_t k) {
	char c = getchar();
	while(c != EOF && c != '\n' && isspace(c)) {
		c = getchar();
	}
	if(c == EOF || c == '\n') {
		fprintf(stderr, "ERROR 2\n");
		return false;
	}
	
	size_t i = 0, current_product = 1;
	while(true) {
		size_t x = 0; // current coordinate
		while(c != EOF && !isspace(c)) {
			if(!('0' <= c && c <= '9') || x > (SIZE_MAX - (c - '0')) / 10) {
				fprintf(stderr, "ERROR 2\n");
				return false;
			}
			x = x * 10 + c - '0';
			c = getchar();
		}
		
		if(i == k || !(1 <= x && x <= dimensions.items[i])) {
			fprintf(stderr, "ERROR 2\n");
			return false;
		}
		*start += (x - 1) * current_product;
		current_product *= dimensions.items[i++];
		
		while(c != EOF && isspace(c)) {
			if(c == '\n') {
				break;
			}
			c = getchar();
		}
		if(c == EOF) {
			if(i < k) {
				fprintf(stderr, "ERROR 2\n");
			}
			else {
				fprintf(stderr, "ERROR 3\n");
			}
			return false;
		}
		else if(c == '\n') {
			break;
		}
	}
	
	if(i != k) {
		fprintf(stderr, "ERROR 2\n");
		return false;
	}
	
	assert(*start < maze_size);
	assert(c == '\n');
	
	return true;
}

bool read_third_line(size_t *end, Vector dimensions, size_t maze_size, size_t k) {
	char c = getchar();
	while(c != EOF && c != '\n' && isspace(c)) {
		c = getchar();
	}
	if(c == EOF || c == '\n') {
		fprintf(stderr, "ERROR 3\n");
		return false;
	}
	
	size_t i = 0, current_product = 1;
	while(true) {
		size_t y = 0; // current coordinate
		while(c != EOF && !isspace(c)) {
			if(!('0' <= c && c <= '9') || y > (SIZE_MAX - (c - '0')) / 10) {
				fprintf(stderr, "ERROR 3\n");
				return false;
			}
			y = y * 10 + c - '0';
			c = getchar();
		}
		
		if(i == k || !(1 <= y && y <= dimensions.items[i])) {
			fprintf(stderr, "ERROR 3\n");
			return false;
		}
		*end += (y - 1) * current_product;
		current_product *= dimensions.items[i++];
		
		while(c != EOF && isspace(c)) {
			if(c == '\n') {
				break;
			}
			c = getchar();
		}
		if(c == EOF) {
			if(i < k) {
				fprintf(stderr, "ERROR 3\n");
			}
			else {
				fprintf(stderr, "ERROR 4\n");
			}
			return false;
		}
		else if(c == '\n') {
			break;
		}
	}
	
	if(i != k) {
		fprintf(stderr, "ERROR 3\n");
		return false;
	}
	
	assert(*end < maze_size);
	assert(c == '\n');
	
	return true;
}

bool read_fourth_line(Bitset *visited, size_t maze_size) {
	char c = getchar();
	while(c != EOF && c != '\n' && isspace(c)) {
		c = getchar();
	}
	if(c == EOF || c == '\n') {
		fprintf(stderr, "ERROR 4\n");
		return false;
	}
	
	if(c == 'R') {
		uint64_t a = 0, b = 0, m = 0, r = 0, s = 0;
		
		c = getchar();
		while(c != EOF && c != '\n' && isspace(c)) {
			c = getchar();
		}
		if(c == EOF || c == '\n') {
			fprintf(stderr, "ERROR 4\n");
			return false;
		}
		
		for(size_t i = 0; i < 5; i++) {
			uint64_t number = 0;
			while(c != EOF && !isspace(c)) {
				if(!('0' <= c && c <= '9') || number > (UINT32_MAX - (c - '0')) / 10) {
					fprintf(stderr, "ERROR 4\n");
					return false;
				}
				number = number * 10 + c - '0';
				c = getchar();
			}
			
			if(i == 0) {
				a = number;
			}
			else if(i == 1) {
				b = number;
			}
			else if(i == 2) {
				if(number == 0) {
					fprintf(stderr, "ERROR 4\n");
					return false;
				}
				m = number;
			}
			else if(i == 3) {
				r = number;
			}
			else {
				assert(i == 4);
				s = number;
			}
			
			while(c != EOF && c != '\n' && isspace(c)) {
				c = getchar();
			}
			if((i < 4 && (c == EOF || c == '\n')) ||
			   (i == 4 && (c != EOF && c != '\n'))) {
				fprintf(stderr, "ERROR 4\n");
				return false;
			}
		}
		if(c != EOF) {
			if(c != '\n') {
				fprintf(stderr, "ERROR 4\n");
				return false;
			}
			else {
				c = getchar();
				if(c != EOF) {
					fprintf(stderr, "ERROR 5\n");
					return false;
				}
			}
		}
		
		// Setting up the blocked cells.
		for(size_t i = 1; i <= r; i++) {
			s = (a * s % m + b) % m;
			
			size_t cell = s % maze_size;
			
			uint64_t jump = 1;
			jump <<= 32;
			if(maze_size <= jump) {
				bitset_set(visited, cell, 1);
			}
			else if(!bitset_get(visited, cell)) {
				// Finding all numbers j, such that j % (1 << 32) == cell.
				for(; cell < maze_size - jump; cell += jump) {
					bitset_set(visited, cell, 1);
				}
				bitset_set(visited, cell, 1);
			}
		}
	}
	else {
		if(c != '0') {
			fprintf(stderr, "ERROR 4\n");
			return false;
		}
		c = getchar();
		if(c != 'x') {
			fprintf(stderr, "ERROR 4\n");
			return false;
		}
		
		c = getchar();
		size_t last_pos = 0;
		bool non_zero = false;
		while(c != EOF && !isspace(c)) {
			size_t value = 0;
			if('0' <= c && c <= '9') {
				value = c - '0';
			}
			else if('a' <= c && c <= 'f') {
				value = 10 + c - 'a';
			}
			else if('A' <= c && c <= 'F') {
				value = 10 + c - 'A';
			}
			else {
				fprintf(stderr, "ERROR 4\n");
				return false;
			}
			
			if(non_zero) {
				size_t bit = 3u;
				while(true) {
					if(last_pos >= maze_size) {
						fprintf(stderr, "ERROR 4\n");
						return false;
					}
					
					if((value >> bit) & 1) {
						bitset_set(visited, last_pos, 1);
					}
					last_pos++;
					
					if(bit == 0) {
						break;
					}
					bit--;
				}
			}
			else if(value != 0) {
				size_t bit = 3u;
				while(!(value >> bit) & 1) {
					assert(bit > 0u);
					bit--;
				}
				while(true) {
					if(last_pos >= maze_size) {
						fprintf(stderr, "ERROR 4\n");
						return false;
					}
					
					if((value >> bit) & 1) {
						bitset_set(visited, last_pos, 1);
					}
					last_pos++;
					
					if(bit == 0) {
						break;
					}
					bit--;
				}
				
				non_zero = true;
			}
			
			c = getchar();
		}
		
		while(c != EOF && c != '\n' && isspace(c)) {
			c = getchar();
		}
		if(c != EOF) {
			if(c != '\n') {
				fprintf(stderr, "ERROR 4\n");
				return false;
			}
			else {
				c = getchar();
				if(c != EOF) {
					fprintf(stderr, "ERROR 5\n");
					return false;
				}
			}
		}
		
		// Reversing the array of bits.
		for(size_t i = 0; i < last_pos / 2; i++) {
			size_t j = last_pos - 1 - i;
			size_t i_value = bitset_get(visited, i), j_value = bitset_get(visited, j);
			if(i_value != j_value) {
				bitset_set(visited, i, j_value);
				bitset_set(visited, j, i_value);
			}
		}
	}
	
	return true;
}

bool bfs(size_t *answer, Bitset *visited, size_t start, size_t end, Vector dimensions, size_t k) {
	Queue queue;
	queue_init(&queue);
	queue_push(&queue, start, 0u);
	
	bitset_set(visited, start, 1);
	
	while(queue.size > 0) {
		size_t vertex = (queue.head)->vertex, distance = (queue.head)->distance;
		queue_pop(&queue);
		
		if(vertex == end) {
			*answer = distance;
			break;
		}
		
		// Visiting neighbours.
		size_t vertex_copy = vertex, current_product = 1;
		for(size_t i = 0; i < k; i++) {
			size_t x = vertex_copy % dimensions.items[i] + 1; // current dimension
			vertex_copy /= dimensions.items[i];
			if(x > 1) {
				size_t neighbour = vertex - current_product;
				if(!bitset_get(visited, neighbour)) {
					bitset_set(visited, neighbour, 1);
					if(!queue_push(&queue, neighbour, distance + 1)) {
						queue_clear(&queue);
						return false;
					}
				}
			}
			if(x < dimensions.items[i]) {
				size_t neighbour = vertex + current_product;
				if(!bitset_get(visited, neighbour)) {
					bitset_set(visited, neighbour, 1);
					if(!queue_push(&queue, neighbour, distance + 1)) {
						queue_clear(&queue);
						return false;
					}
				}
			}
			current_product *= dimensions.items[i];
		}
	}
	
	queue_clear(&queue);
	
	return true;
}

int main() {
	Vector dimensions;
	if(!vector_init(&dimensions)) {
		vector_clear(&dimensions);
		return 1;
	}
	size_t maze_size = 1;
	if(!read_first_line(&dimensions, &maze_size)) {
		vector_clear(&dimensions);
		return 1;
	}
	size_t k = dimensions.size; // the number of dimensions
	
	size_t start = 0;
	if(!read_second_line(&start, dimensions, maze_size, k)) {
		vector_clear(&dimensions);
		return 1;
	}
	
	size_t end = 0;
	if(!read_third_line(&end, dimensions, maze_size, k)) {
		vector_clear(&dimensions);
		return 1;
	}
	
	Bitset visited;
	if(!bitset_init(&visited, maze_size)) {
		vector_clear(&dimensions);
		bitset_clear(&visited);
		return 1;
	}
	
	if(!read_fourth_line(&visited, maze_size)) {
		vector_clear(&dimensions);
		bitset_clear(&visited);
		return 1;
	}
	
	// Checking if the starting cell is vacant.
	if(bitset_get(&visited, start)) {
		fprintf(stderr, "ERROR 2\n");
		vector_clear(&dimensions);
		bitset_clear(&visited);
		return 1;
	}
	// Checking if the ending cell is vacant.
	if(bitset_get(&visited, end)) {
		fprintf(stderr, "ERROR 3\n");
		vector_clear(&dimensions);
		bitset_clear(&visited);
		return 1;
	}
	
	size_t answer = 0;
	if(!bfs(&answer, &visited, start, end, dimensions, k)) {
		vector_clear(&dimensions);
		bitset_clear(&visited);
		return 1;
	}
	
	// If the ending cell is not visited, then no way exists.
	if(!bitset_get(&visited, end)) {
		printf("NO WAY\n");
	}
	else {
		assert(answer < maze_size);
		printf("%zu\n", answer);
	}
	
	vector_clear(&dimensions);
	bitset_clear(&visited);
	
	return 0;
}