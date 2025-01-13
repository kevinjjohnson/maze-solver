#include "include/maze.h"
#include <time.h>
#include "include/priority_queue.h"

void init_maze(maze* maze, int size) {
	maze->size = size;
	maze->cells = malloc(size * size * sizeof(int));
	int curr_wall = 0;
	for (int i = 0; i < size * size; i++) {
		int row = (i % size);
		int col = i / size;
		if (row % 2 == 1 && col % 2 == 1)
			maze->cells[i] = 0;
		else
			maze->cells[i] = 1;
	}
}

void free_maze(maze* maze) {
	free(maze->cells);
}

void generate_maze(maze* maze) {
	disjoint_set open_cells;
	init_disjoint_set(&open_cells, maze->size * maze->size);

	int walls_size = (maze->size * maze->size) / 2;
	int* walls = malloc(walls_size * sizeof(int));
	int curr_wall = 0;
	for (int i = 1; i < maze->size * maze->size; i += 2) {
		walls[curr_wall] = i;
		curr_wall++;
	}
	random_sort_walls(walls, walls_size);
	
	for (int i = 0; i < walls_size; i++) {
		int wall = walls[i];
		int col = wall % maze->size;
		int row = wall / maze->size;
		int x, y;
		x = -1;
		y = -1;
		
		if (col >= 1 && col <= maze->size - 2 && maze->cells[wall - 1] == 0 && maze->cells[wall + 1] == 0) {
			x = find(&(open_cells), wall - 1);
			y = find(&(open_cells), wall + 1);
		}
		else if (row >= 1 && row <= maze->size - 2 && maze->cells[wall - maze->size] == 0 && maze->cells[wall + maze->size] == 0) {
			x = find(&(open_cells), wall - maze->size);
			y = find(&(open_cells), wall + maze->size);
		}
		if (x != -1 && y != -1) {
			if (x != y) {
				maze->cells[wall] = 0;
				join_set(&(open_cells), x, y);
			}
		}
	}
	maze->cells[maze->size - 2] = 0;
	maze->cells[maze->size * maze->size - maze->size + 1] = 0;

	free_disjoint_set(&open_cells);
	free(walls);
}

void render_maze(batch_renderer* renderer, maze* maze, float cell_size) {
	int offset = 60;
	for (int i = 0; i < maze->size * maze->size; i++) {
		quad q;
		int row = i / maze->size;
		int col = i % maze->size;
		init_quad(&q, (vec2) { cell_size, cell_size }, (vec2) {col * cell_size + offset, row * cell_size + offset}, maze->cells[i]);
		add_quad(renderer, q);
	}
}

void print_maze(maze* maze){
	for (int i = 0; i < maze->size * maze->size; i++) {
		printf("%d", maze->cells[i]);
		if (i % maze->size == maze->size - 1) printf("\n");
	}
}

void random_sort_walls(int* walls, int size) {
	srand(time(NULL));
	int temp, random;
	for (int i = 0; i < size; i++) {
		random = (int) ((double)rand() / ((double)RAND_MAX + 1) * size);
		temp = walls[random];
		walls[random] = walls[i];
		walls[i] = temp;
	}
}

void solve_maze(maze* maze, int start, int end) {
	priority_queue queue;
	init_priority_queue(&queue, maze->size * maze->size / 2);
	int size = maze->size * maze->size;
	int* parent = malloc(size * sizeof(int));
	int* distance = malloc(size * sizeof(int));

	for (int d = 0; d < size; d++) {
		distance[d] = -1;
	}


	//first element of maze
	queue_element e;
	e.element = start;
	e.priority = 0;
	queue_insert(&queue, e);
	distance[start] = 0;
	parent[start] = -1;

	while (queue.size > 0) {
		//print_queue(queue);
		int current_cell = queue_extract_min(&queue);
		if (current_cell == end) break;

		int row = current_cell % maze->size;
		int col = current_cell / maze->size;

		if (row > 0) {
			if (maze->cells[current_cell - maze->size] == 0 && distance[current_cell - maze->size] == -1) {
				parent[current_cell - maze->size] = current_cell;
				distance[current_cell - maze->size] = distance[current_cell] + 1;
				queue_element top;
				top.element = current_cell - maze->size;
				top.priority = distance[current_cell] + 1;
				queue_insert(&queue, top);
			}
		}
		if (row < maze->size - 1) {
			if (maze->cells[current_cell + maze->size] == 0 && distance[current_cell + maze->size] == -1) {
				parent[current_cell + maze->size] = current_cell;
				distance[current_cell + maze->size] = distance[current_cell] + 1;
				queue_element bottom;
				bottom.element = current_cell + maze->size;
				bottom.priority = distance[current_cell] + 1;
				queue_insert(&queue, bottom);
			}
		}
		if (col > 0) {
			if (maze->cells[current_cell - 1] == 0 && distance[current_cell - 1] == -1) {
				parent[current_cell - 1] = current_cell;
				distance[current_cell - 1] = distance[current_cell] + 1;
				queue_element bottom;
				bottom.element = current_cell - 1;
				bottom.priority = distance[current_cell] + 1;
				queue_insert(&queue, bottom);
			}
		}
		if (col < maze->size - 1) {
			if (maze->cells[current_cell + 1] == 0 && distance[current_cell + 1] == -1) {
				parent[current_cell + 1] = current_cell;
				distance[current_cell + 1] = distance[current_cell] + 1;
				queue_element bottom;
				bottom.element = current_cell + 1;
				bottom.priority = distance[current_cell] + 1;
				queue_insert(&queue, bottom);
			}
		}

	}

	/*
	for (int i = 0; i < size; i++) {
		printf("cell %d, parent: %d\n", i, parent[i]);
	}
	*/
	int index = end;
	while (index != -1) {
		maze->cells[index] = 2;
		index = parent[index];
	}

	free(parent);
	free(distance);
	free_priority_queue(&queue);
}

