#include "util.h"

int **read_file(char *filename, int *n) {
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

    int **graph;
    graph = (int **)calloc(*n, sizeof(int *));
    for (int i = 0; i < *n; i++) {
        graph[i] = (int *)calloc(*n, sizeof(int));
    }
    for (int i = 0; i < m; i++) {
        int v, w;
        if (!fgets(line, BUFFER_SZ, f)) return 0;
        
        p = strtok(line, " ");
        if (!p) return NULL;
        v = atoi(p);

        p = strtok(NULL, " ");
        if (!p) return NULL;
        w = atoi(p);

        graph[v][w] = 1;
        graph[w][v] = 1;
    }

    fclose(f);
    //return 0;
    return graph;
}

// int main(int argc, char *argv[]){
//     char *v;
//     char *e;
//     read_file("data_example/1.txt", &v, &e);
//     printf("Vertex = %s\n", v);
//     printf("Edges = %s\n\n", e);


//     int m;
//     int n;

//     calc_m_n(v, e, &m, &n);

//     printf("m = %d, n = %d\n", m, n);
//     free(v);
//     free(e);
//     return 0;
// }