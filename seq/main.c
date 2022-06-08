#include <time.h>
#include "util.h"


int main(int argc, char **argv) {
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

    /////////////////////////////
    // generating random weights
    /////////////////////////////
    int w[n];
    srand(time(NULL)); // seed of rand
    printf("w = ");
    for (int i = 0; i < n; i++) {
        w[i] = rand() % 10000;
        printf("%d ", w[i]);
    }
    printf("\n");

    ////////////
    // coloring
    ////////////
    int color[n];
    for (int i = 0; i < n; i++) color[i] = -1;
    int colored_count = 0;

    int set_I[n];
    int size_I = 0;
    int **resp;

    while (colored_count < n) {
        for (int v = 0; v < n; v++) {
            if (color[v] != -1) continue; // already colored

            resp = get_uncolored_neighbours(graph, n, v, color);
            int neighbours_size = *resp[0];
            int *neighbours = resp[1];

            ///////////////
            // Build Set I
            ///////////////
            //printf("Uncolored Neighbours of %d: ", v);
            int add_to_set_i = 1;
            for (int i = 0; i < neighbours_size; i++) {
                //printf("%d ", neighbours[i]);
                int u = neighbours[i];
                if (w[v] < w[u]) {
                    add_to_set_i = 0;
                    break;
                }
            }
            //printf("\n");

            if (add_to_set_i) {
                set_I[size_I] = v;
                size_I++;
            }

            free(resp[0]);
            free(resp[1]);
            free(resp);

            //printf("Set I: ");
            for (int i = 0; i < size_I; i++) {
                //printf("%d ", set_I[i]);
                int v_i = set_I[i];
                resp = get_neighbours_color(graph, n, v_i, color);
                int neighbours_colors_size = *resp[0];
                int *neighbours_colors = resp[1];

                int max_color = -1;
                for (int j = 0; j < neighbours_colors_size; j++) {
                    if (neighbours_colors[j] > max_color) max_color = neighbours_colors[j];
                }
                
                max_color++;
                color[v_i] = max_color;
                colored_count++;

                free(resp[0]);
                free(resp[1]);
                free(resp);
            }
            size_I = 0;
            //printf("\n");
        }
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