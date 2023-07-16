#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct QueueNode {
	size_t vertex, distance;
	struct QueueNode *next;
} QueueNode;

typedef struct Queue {
	size_t size;
	QueueNode *head, *tail;
} Queue;

void queue_init(Queue *);

bool queue_push(Queue *, size_t, size_t);

void queue_pop(Queue *);

void queue_clear(Queue *);
