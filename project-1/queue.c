#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include "queue.h"

void queue_init(Queue *queue) {
	queue->size = 0;
	queue->head = queue->tail = NULL;
}

bool queue_push(Queue *queue, size_t vertex, size_t distance) {
	QueueNode *tmp = malloc(sizeof(QueueNode));
	if(tmp == NULL) {
		fprintf(stderr, "ERROR 0\n");
		return false;
	}
	tmp->vertex = vertex;
	tmp->distance = distance;
	tmp->next = NULL;
	
	if(queue->size == 0) {
		assert(queue->head == NULL && queue->tail == NULL);
		
		tmp->next = NULL;
		queue->head = queue->tail = tmp;
		queue->size = 1;
	}
	else {
		assert(queue->head != NULL && queue->tail != NULL);
		
		(queue->tail)->next = tmp;
		queue->tail = tmp;
		queue->size++;
	}
	
	return true;
}

void queue_pop(Queue *queue) {
	assert(queue->size > 0);
	assert(queue->head != NULL && queue->tail != NULL);
	
	QueueNode *tmp = queue->head;
	queue->head = (queue->head)->next;
	free(tmp);
	
	if(--queue->size == 0) {
		assert(queue->head == NULL);
		queue->tail = NULL;
	}
}

void queue_clear(Queue *queue) {
	while(queue->size > 0) {
		queue_pop(queue);
	}
}
