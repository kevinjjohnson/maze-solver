#include "include/priority_queue.h"
#include <stdlib.h>

void init_priority_queue(priority_queue* queue, int max_size) {
	queue->elements = malloc(max_size * sizeof(queue_element));
	queue->size = 0;
}

int parent_index(int index) {
	return (index - 1) / 2;
}

int left_child_index(int index) {
	return (2 * index) + 1;
}

int right_child_index(int index) {
	return (2 * index) + 2;
}

void shift_node_up(priority_queue* queue, int i) {
	int index = i;
	while (index > 0 && queue->elements[parent_index(index)].priority < queue->elements[index].priority) {
		queue_element temp;
		temp.priority = queue->elements[parent_index(index)].priority;
		temp.element = queue->elements[parent_index(index)].element;
		queue->elements[parent_index(index)].priority = queue->elements[index].priority;
		queue->elements[parent_index(index)].element = queue->elements[index].element;
		queue->elements[index].priority = temp.priority;
		queue->elements[index].element = temp.element;
		index = parent_index(index);
	}
}

void shift_node_down(priority_queue* queue, int index) {
	int max = index;

	int left = left_child_index(index);
	if (left < queue->size && queue->elements[left].priority > queue->elements[max].priority) {
		max = left;
	}

	int right = right_child_index(index);
	if (right < queue->size && queue->elements[right].priority > queue->elements[max].priority) {
		max = right;
	}

	if (index != max) {
		queue_element temp;
		temp.priority = queue->elements[max].priority;
		temp.element = queue->elements[max].element;
		queue->elements[max].priority = queue->elements[index].priority;
		queue->elements[max].element = queue->elements[index].element;
		queue->elements[index].priority = temp.priority;
		queue->elements[index].element = temp.element;
		shift_node_down(queue, max);
	}
}

int queue_extract_min(priority_queue* queue) {
	int result = queue->elements[0].element;
	queue->elements[0].priority = queue->elements[queue->size - 1].priority;
	queue->elements[0].element = queue->elements[queue->size - 1].element;
	queue->size -= 1;
	shift_node_down(queue, 0);
}

void queue_insert(priority_queue* queue, queue_element element) {
	queue->elements[queue->size].element = element.element;
	queue->elements[queue->size].priority = element.priority;
	queue->size += 1;
	shift_node_up(queue, queue->size - 1);
}

void print_queue(priority_queue queue){
	for (int i = 0; i < queue.size; i++) {
		printf("Node with priority: %d, and contents: %d\n", queue.elements[i].priority, queue.elements[i].element);
	}
}
