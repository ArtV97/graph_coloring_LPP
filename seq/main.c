#include <time.h>
#include "util.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Missing argument <filename>\n");
        return 1;
    }

    // graph variables
    Graph *graph = NULL; // matrix representing the graph
    char *filename = argv[1]; // file containing the graph

    //////////////////////
    // initializing graph
    //////////////////////
    graph = read_file(filename);
    if (!graph) {
        printf("Error reading graph\n");
        return 1;
    }

    ////////////
    // coloring
    ////////////
    int color[graph->n];
    for (int i = 0; i < graph->n; i++) color[i] = -1;
    int colored_count = 0;
    
    int **resp; // used to receive awnser of neighbours function

    clock_t initial = clock();
    for (int v = 0; v < graph->n; v++) {
        
        resp = get_neighbours_color(graph, v, color);
        int neighbours_colors_size = *resp[0];
        int *neighbours_colors = resp[1];

        
        ///////////////////////
        // Choosing color of v
        ///////////////////////       
        int min_color = 0;
        int prev_min_color = -1;
        while (prev_min_color != min_color) {
            prev_min_color = min_color;
            for (int i = 0; i < neighbours_colors_size; i++) {
                if (neighbours_colors[i] == min_color) {
                    min_color++;
                    break;
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
    
    printf("Colors: ");
    for (int i = 0; i < graph->n; i++) {
        printf("%d ", color[i]);
    }
    printf("\n");

	printf("Calculation Execution Time: %lf\n", result);    

    graph_destroy(graph);
    return 0;
}