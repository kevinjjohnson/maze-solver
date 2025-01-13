#pragma once
#include "maze_generator.h"

typedef struct {
	int* path;
	int size;
}maze_solution;

maze_solution solve_maze(maze* maze, int start, int end);
