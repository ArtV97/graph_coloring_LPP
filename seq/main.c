#include <time.h>
#include "util.h"

#define UNCHOOSE_COLOR 9999999

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Missing argument <filename>\n");
        return 1;
    }

    // graph variables
    int *graph = NULL; // matrix representing the graph
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

    clock_t initial = clock();
    for (int v = 0; v < n; v++) {
        
        resp = get_neighbours_color(graph, n, v, color);
        int neighbours_colors_size = *resp[0];
        int *neighbours_colors = resp[1];

        int min_color = UNCHOOSE_COLOR;
        for (int j = 0; j < neighbours_colors_size; j++) {
            if (neighbours_colors[j] < min_color) min_color = neighbours_colors[j];
        }
        
        ///////////////////////
        // Choosing color of v
        ///////////////////////
        if (min_color == UNCHOOSE_COLOR) {
            min_color = 0;
        }
        else if (min_color - 1 >= 0) {
            min_color--;
        }
        else {
            min_color = 1;
            int prev_min_color = 0;
            while (prev_min_color != min_color) {
                prev_min_color = min_color;
                for (int i = 0; i < neighbours_colors_size; i++) {
                    if (neighbours_colors[i] == min_color) {
                        min_color++;
                        break;
                    }
                }
            }
        }

        color[v] = min_color;

        free(resp[0]);
        free(resp[1]);
        free(resp);
    }
    clock_t final = clock();
    double result = ((double)(final-initial)/CLOCKS_PER_SEC);
	printf("Calculation Execution Time: %lf\n", result);    
    
    printf("Colors: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", color[i]);
    }
    printf("\n");

    // for (int i = 0; i < n; i++) {
    //     free(graph[i]);
    // }
    // free(graph);

    free(graph);

    return 0;
}