#include "include/maze_solver.h"
#include "include/priority_queue.h"

maze_solution solve_maze(maze* maze, int start, int end) {
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
