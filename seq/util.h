#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 1024

int **read_file(char *filename, int *n);

int **get_uncolored_neighbours(int **graph, int n, int v, int *color);

int **get_neighbours_color(int **graph, int n, int v, int *color);
