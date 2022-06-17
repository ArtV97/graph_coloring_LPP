#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
    int *graph;
    int *map;
    int n;
} Graph;


Graph *graph_init(int n);
int graph_get_val(Graph *g, int v, int w);
void graph_set_val(Graph *g, int v, int w, int val);
void graph_destroy(Graph *g);