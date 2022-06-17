#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"

#define BUFFER_SZ 1024

Graph *read_file(char *filename, int *n);

int **get_uncolored_neighbours(Graph *graph, int n, int v, int *color);

int **get_neighbours_color(Graph *graph, int n, int v, int *color);
