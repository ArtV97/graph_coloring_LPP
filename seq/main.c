#include "util.h"

int *PE = NULL;
int *PS = NULL;
int *pai = NULL;
int *cor = NULL;

int coloring(int **graph, int n, int v, int t) {
    t++;
    PE[v] = t;

    for (int w = 0; w < n; w++) {
        if (!graph[v][w]) continue;

        if (PE[w] == 0) {
            pai[w] = v;
            cor[w] = 1 - cor[v];
            if (coloring(graph, n, w, t)) return 1;
        }
        else if (PS[w] == 0 && w != pai[v]) {
            if (cor[w] != cor[v]) continue;
            else {
                printf("Nao eh 2-colorivel\n");
                return 1;
            }
        }
    }
    t++;
    PS[v] = t;
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Missing argument <filename>\n");
        return 1;
    }

    char *filename = argv[1];
    int n;
    int **graph = read_file(filename, &n);
    if (!graph) {
        printf("Error reading graph\n");
        return 0;
    }
    
    PE = (int *)calloc(n, sizeof(int));
    PS = (int *)calloc(n, sizeof(int));
    pai = (int *)calloc(n, sizeof(int));
    cor = (int *)calloc(n, sizeof(int));

    if (!coloring(graph, n, 0, 0)) {
        printf("Eh 2-colorivel\n");
    }
    
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }

    free(graph);
    free(PE);
    free(PS);
    free(pai);
    free(cor);

    return 0;
}