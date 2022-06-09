#include <time.h>
#include "util.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Missing argument <filename>\n");
        return 1;
    }

    // graph variables
    int **graph = NULL; // matrix representing the graph
    int n; // number of vertex

    char *filename = argv[1]; // file containing the graph

    //////////////////////
    // initializing graph
    //////////////////////
    graph = read_file(filename, &n);
    if (!graph) {
        printf("Error reading graph\n");
        return 1;
    }

    ////////////
    // coloring
    ////////////
    int color[n];
    for (int i = 0; i < n; i++) color[i] = -1;
    int colored_count = 0;
    
    int **resp; // used to receive awnser of neighbours function

    for (int v = 0; v < n; v++) {
        
        resp = get_neighbours_color(graph, n, v, color);
        int neighbours_colors_size = *resp[0];
        int *neighbours_colors = resp[1];

        int max_color = -1;
        for (int j = 0; j < neighbours_colors_size; j++) {
            if (neighbours_colors[j] > max_color) max_color = neighbours_colors[j];
        }
        
        max_color++;
        color[v] = max_color;

        free(resp[0]);
        free(resp[1]);
        free(resp);
    }
    
    printf("Colors: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", color[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph); 

    return 0;
}