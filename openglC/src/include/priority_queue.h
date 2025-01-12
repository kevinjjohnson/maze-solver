#pragma once

typedef struct {
	int priority;
	int element;
}queue_element;

typedef struct {
	int size;
	queue_element* elements;
}priority_queue;

void init_priority_queue(priority_queue* queue, int max_size);
void free_priority_queue(priority_queue* queue);

int queue_extract_min(priority_queue* queue);
void queue_insert(priority_queue* queue, queue_element element);
void print_queue(priority_queue queue);