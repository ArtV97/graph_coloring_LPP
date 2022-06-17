#include "util.h"

Graph *read_file(char *filename, int *n) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return NULL;

    char line[BUFFER_SZ+1];
    char *p;
    int m; // n vertex, m edges

    if (!fgets(line, BUFFER_SZ, f)) return 0;
    p = strtok(line, " ");
    if (!p) return NULL;        
    *n = atoi(p);
    
    p = strtok(NULL, " ");
    if (!p) return NULL;
    m = atoi(p);

    //int *graph = calloc((*n)*(*n), sizeof(int));
    Graph *g = graph_init(*n);
    
    for (int i = 0; i < m; i++) {
        int v, w;
        if (!fgets(line, BUFFER_SZ, f)) return 0;
        
        p = strtok(line, " ");
        if (!p) return NULL;
        v = atoi(p);

        p = strtok(NULL, " ");
        if (!p) return NULL;
        w = atoi(p);

        //graph[w+(v*(*n))] = 1;
        //graph[v+(w*(*n))] = 1;
        graph_set_val(g, v, w, 1);
    }

    fclose(f);
    //return 0;
    //return graph;
    return g;
}

int get_graph_val(int *graph, int v, int w, int n) {
    return graph[w+(v*n)];
}

int **get_uncolored_neighbours(Graph *graph, int n, int v, int *color) {
    int *count = calloc(1, sizeof(int));
    int *neighbours = malloc(n*sizeof(int));
    for (int w = 0; w < n; w++) {
        //if (color[w] == -1 && graph[v][w]) {
        //if (color[w] == -1 && get_graph_val(graph, v, w, n)) {
        if (color[w] == -1 && graph_get_val(graph, v, w)) {
            count[0]++;
            neighbours[count[0]-1] = w;
        }
    }
    
    neighbours = realloc(neighbours, count[0]*sizeof(int));
    int **resp = malloc(2*sizeof(int*));
    resp[0] = count;
    resp[1] = neighbours;

    return resp;
}

int **get_neighbours_color(Graph *graph, int n, int v, int *color) {
    int *count = calloc(1, sizeof(int));
    int *neighbours_colors = malloc(n*sizeof(int));
    for (int w = 0; w < n; w++) {
        //if (color[w] != -1 && graph[v][w]) {
        //if (color[w] != -1 && get_graph_val(graph, v, w, n)) {
        if (color[w] != -1 && graph_get_val(graph, v, w)) {
            count[0]++;
            neighbours_colors[count[0]-1] = color[w];
        }
    }
    
    neighbours_colors = realloc(neighbours_colors, count[0]*sizeof(int));
    int **resp = malloc(2*sizeof(int*));
    resp[0] = count;
    resp[1] = neighbours_colors;

    return resp;
}


// int main(int argc, char *argv[]){
//     int n;
//     int **graph;
//     graph = read_file("../data_example/2.txt", &n);

//     int **resp = get_neighbours(graph, n, 3);
//     printf("resp[0] = %d\n", *resp[0]);
//     for (int i = 0; i < *resp[0]; i++) {
//         printf("resp[1][%d] = %d\n", i, resp[1][i]);
//     }

//     for (int i = 0; i < n; i++) {
//         free(graph[i]);
//     }
//     free(graph);
//     return 0;
// }