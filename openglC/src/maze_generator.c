#include "include/maze_generator.h"

void init_maze(maze* maze, int size) {
	maze->size = size;
	maze->cells = malloc(size * size * sizeof(int));
	maze->walls_size = (size * size) / 2;
	maze->walls = malloc(maze->walls_size * sizeof(int));
	int curr_wall = 0;
	for (int i = 0; i < size * size; i++) {
		if (i % 2 == 1) {
			maze->cells[i] = 1;
			maze->walls[curr_wall] = i;
			curr_wall++;
		}
		else {
			int row = (i % size);
			int col = i / size;
			if (row % 2 == 1 && col % 2 == 1)
				maze->cells[i] = 0;
			else
				maze->cells[i] = 1;
		}
	}
}

void free_maze(maze* maze) {
}

void generate_maze(maze* maze) {
}

void render_maze(batch_renderer* renderer, maze* maze, float cell_size) {
	int offset = 60;
	for (int i = 0; i < maze->size * maze->size; i++) {
		quad q;
		int row = i % maze->size;
		int col = i / maze->size;
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

void random_sort_walls(maze* maze) {
	int temp, random;
	for (int i = 0; i < maze->walls_size; i++) {
		random = (int) ((double)rand() / ((double)RAND_MAX + 1) * maze->walls_size);
		temp = maze->walls[random];
		maze->walls[random] = maze->walls[i];
		maze->walls[i] = temp;
	}
}
