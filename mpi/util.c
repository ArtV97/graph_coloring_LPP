#include "util.h"

int *read_file(char *filename, int *n) {
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

    // int **graph;
    // graph = (int **)calloc(*n, sizeof(int *));
    // for (int i = 0; i < *n; i++) {
    //     graph[i] = (int *)calloc(*n, sizeof(int));
    // }
    int *graph = calloc((*n)*(*n), sizeof(int));
    
    for (int i = 0; i < m; i++) {
        int v, w;
        if (!fgets(line, BUFFER_SZ, f)) return 0;
        
        p = strtok(line, " ");
        if (!p) return NULL;
        v = atoi(p);

        p = strtok(NULL, " ");
        if (!p) return NULL;
        w = atoi(p);

        //graph[v][w] = 1;
        graph[w+(v*(*n))] = 1;
        //graph[w][v] = 1;
        graph[v+(w*(*n))] = 1;
    }

    fclose(f);
    //return 0;
    return graph;
}

int get_graph_val(int *graph, int v, int w, int n) {
    return graph[w+(v*n)];
}

int **get_uncolored_neighbours(int *graph, int n, int v, int *color) {
    int *count = calloc(1, sizeof(int));
    int *neighbours = malloc(n*sizeof(int));
    for (int w = 0; w < n; w++) {
        //if (color[w] == -1 && graph[v][w]) {
            if (color[w] == -1 && get_graph_val(graph, v, w, n)) {
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

int **get_neighbours_color(int *graph, int n, int v, int *color) {
    int *count = calloc(1, sizeof(int));
    int *neighbours_colors = malloc(n*sizeof(int));
    for (int w = 0; w < n; w++) {
        //if (graph[v][w]) {
        if (get_graph_val(graph, v, w, n)) {
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