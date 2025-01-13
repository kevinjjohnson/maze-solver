#include "include/disjoint_set.h"
#include <stdlib.h>

void init_disjoint_set(disjoint_set* set, int size){
	set->size = size;
	set->parent = malloc(size * sizeof(int));
	set->rank = malloc(size * sizeof(int));
	for (int i = 0; i < size; i++) {
		set->parent[i] = i;
		set->rank[i] = 0;
	}
}

void free_disjoint_set(disjoint_set* set) {
	free(set->parent);
	free(set->rank);
}

int find(disjoint_set* set, int i) {
	if (set->parent[i] != i)
		set->parent[i] = find(set, set->parent[i]);
	return set->parent[i];
}

void join_set(disjoint_set* set, int x, int y) {
	int left_root = find(set, x);
	int right_root = find(set, y);

	if (left_root == right_root) return;

	int left_rank = set->rank[left_root];
	int right_rank = set->rank[right_root];

	if (left_rank < right_rank) {
		set->parent[left_root] = right_root;
	}
	else if (right_rank < left_rank) {
		set->parent[right_root] = left_root;
	}
	else {
		set->parent[right_root] = left_root;
		set->rank[left_root] += 1;
	}
}

void print_disjoint_set(disjoint_set* set){
	for (int i = 0; i < set->size; i++) {
		if (set->parent[i] == i) {
			printf("Set with parent %d: {", i);
			for (int j = 0; j < set->size; j++) {
				if (find(set, j) == i)
					printf("%d,", j);
			}
			printf("}\n");
		}
	}
}
