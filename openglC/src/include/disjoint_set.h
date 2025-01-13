#pragma once
#include <stdio.h>

typedef struct {
	int size;
	int* parent;
	int* rank;
}disjoint_set;

void init_disjoint_set(disjoint_set* set, int size);
void free_disjoint_set(disjoint_set* set);
int find(disjoint_set* set, int i);
void join_set(disjoint_set* set, int x, int y);
void print_disjoint_set(disjoint_set* set);