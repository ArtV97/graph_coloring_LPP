#include <time.h>
#include <omp.h>
#include "util.h"

#define UNCHOOSE_COLOR 9999999

int main(int argc, char **argv) {
    // graph variables
    Graph *graph = NULL; // matrix representing the graph
    int n; // number of vertex

    if (argc < 3) {
        printf("Missing arguments.\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);
    char *filename = argv[2]; // file containing the graph

    //////////////////////
    // initializing graph
    //////////////////////
    graph = read_file(filename, &n);
    if (!graph) {
        printf("Error reading graph\n");
        return 1;
    }
    //int num_threads = 2+(n / 1000);
    omp_set_num_threads(num_threads);

    /////////////////////////////
    // generating random weights
    /////////////////////////////
    int w[n];
    srand(time(NULL)); // seed of rand
    //printf("w = ");

    clock_t initial = clock();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        w[i] = rand() % 10000;
        //printf("%d ", w[i]);
    }
    //printf("\n");

    ////////////
    // coloring
    ////////////
    int color[n];
    for (int i = 0; i < n; i++) color[i] = -1;
    int colored_count = 0;

    int set_I[n];
    int size_I = 0;

    while (colored_count < n) {

        #pragma omp parallel for
        for (int v = 0; v < n; v++) {
            if (color[v] != -1) continue; // already colored
            //printf("v: %d thread: %d\n", v, omp_get_thread_num());

            int **resp = get_uncolored_neighbours(graph, n, v, color);
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
                #pragma omp critical
                {
                    set_I[size_I] = v;
                    size_I++;
                }
            }

            free(resp[0]);
            free(resp[1]);
            free(resp);
            //printf("\n");
        }

        //printf("Set I: ");
        #pragma omp parallel for
        for (int i = 0; i < size_I; i++) {
            //printf("%d ", set_I[i]);

            int v_i = set_I[i];
            int **resp = get_neighbours_color(graph, n, v_i, color);
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

            color[v_i] = min_color;

            #pragma omp atomic
            colored_count++;

            free(resp[0]);
            free(resp[1]);
            free(resp);
        }
        size_I = 0;
    }
    
    clock_t final = clock();
    double result = ((double)(final-initial)/CLOCKS_PER_SEC);

    printf("Colors: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", color[i]);
    }
    printf("\n");

	printf("Calculation Execution Time: %lf\n", result);     
    graph_destroy(graph);

    return 0;
}