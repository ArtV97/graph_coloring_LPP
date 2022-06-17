#include "graph.h"


Graph *graph_init(int n) {
    Graph *g = malloc(sizeof(Graph));
    g->map = malloc(n*sizeof(int));
    g->n = n;

    g->map[0] = 1;
    int total_sum = 1;
    for (int i = 1; i < n; i++) {
        g->map[i] = g->map[i-1] + i + 1;
        total_sum = total_sum + i + 1;
    }

    int graph_array_size = n*n - total_sum;
    printf("n*n = %d | total_sum = %d | graph_array_size = %d\n", n*n, total_sum, graph_array_size);

    g->graph = calloc(graph_array_size, sizeof(int));

    return g;
}


int graph_get_val(Graph *g, int v, int w) {
    // j + (i*n) - map[i]
    int pos;
    if (v < w) pos = w + (v*g->n) - g->map[v];
    else pos = v + (w*g->n) - g->map[w];

    //printf("Get pos: %d\n", pos);
    return g->graph[pos];
}

void graph_set_val(Graph *g, int v, int w, int val) {
    int pos;
    if (v < w) pos = w + (v*g->n) - g->map[v];
    else pos = v + (w*g->n) - g->map[w];

    //printf("Set pos: %d\n", pos);
    g->graph[pos] = val;
}

void graph_destroy(Graph *g) {
    free(g->graph);
    free(g->map);
    free(g);
}

void test(Graph *g, int v, int w) {
    printf("Graph[%d][%d]\n", v, w);
    graph_set_val(g, v, w, 1);
    graph_get_val(g, v, w);
    printf("==================\n");
}

// int main() {
//     Graph *g = graph_init(7);

//     test(g, 1, 4);
//     test(g, 2, 6);
//     test(g, 4, 5);
//     test(g, 6, 2);

//     graph_destroy(g);
// }