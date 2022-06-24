#include "util.h"

Graph *read_file(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return NULL;

    char line[BUFFER_SZ+1];
    char *p;
    int n; // n vertex
    int m; // m edges

    if (!fgets(line, BUFFER_SZ, f)) return 0;
    p = strtok(line, " ");
    if (!p) return NULL;        
    n = atoi(p);
    
    p = strtok(NULL, " ");
    if (!p) return NULL;
    m = atoi(p);

    Graph *g = graph_init(n);
    
    for (int i = 0; i < m; i++) {
        int v, w;
        if (!fgets(line, BUFFER_SZ, f)) return 0;
        
        p = strtok(line, " ");
        if (!p) return NULL;
        v = atoi(p);

        p = strtok(NULL, " ");
        if (!p) return NULL;
        w = atoi(p);

        graph_set_val(g, v, w, 1);
    }

    fclose(f);
    return g;
}


int **get_uncolored_neighbours(Graph *g, int v, int *color) {
    int *count = calloc(1, sizeof(int));
    int *neighbours = malloc(g->n*sizeof(int));
    for (int w = 0; w < g->n; w++) {
        if (color[w] == -1 && graph_get_val(g, v, w)) {
            neighbours[count[0]] = w;
            count[0]++;
        }
    }
    
    neighbours = realloc(neighbours, count[0]*sizeof(int));
    int **resp = malloc(2*sizeof(int*));
    resp[0] = count;
    resp[1] = neighbours;

    return resp;
}

int **get_neighbours_color(Graph *g, int v, int *color) {
    int *count = calloc(1, sizeof(int));
    int *neighbours_colors = malloc(g->n*sizeof(int));
    for (int w = 0; w < g->n; w++) {
        if (color[w] != -1 && graph_get_val(g, v, w)) {
            neighbours_colors[count[0]] = color[w];
            count[0]++;
        }
    }
    
    neighbours_colors = realloc(neighbours_colors, count[0]*sizeof(int));
    int **resp = malloc(2*sizeof(int*));
    resp[0] = count;
    resp[1] = neighbours_colors;

    return resp;
}
