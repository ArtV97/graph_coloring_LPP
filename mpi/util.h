#include <string.h>
#include <stdlib.h>
#include "graph.h"

#define BUFFER_SZ 1024

Graph *read_file(char *filename);

int **get_uncolored_neighbours(Graph *g, int v, int *color);

int **get_neighbours_color(Graph *g, int v, int *color);
