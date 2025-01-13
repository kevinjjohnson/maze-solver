#pragma once
#include "disjoint_set.h"
#include "batch_renderer.h"

typedef struct {
	int size;
	int* cells;
}maze;

void init_maze(maze* maze, int size);
void free_maze(maze* maze);
void generate_maze(maze* maze);
void render_maze(batch_renderer* renderer, maze* maze, float cell_size);
void print_maze(maze* maze);
void random_sort_walls(int* walls, int size);
void solve_maze(maze* maze, int start, int end);