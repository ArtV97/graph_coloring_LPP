#include <time.h>
#include "mpi.h"
#include "util.h"

#define ERROR_READING_GRAPH 1


#define TAG_GET_N_V         0   // get neighbours of vertex v
#define TAG_COLORS_N_V      1   // get colors of neighbours of vertex v
#define TAG_COLOR_VI        2   // color vertex Vi of the set_I with color c
#define TAG_FINESHED        3   // process finished processing


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Missing argument <filename>\n");
        return 1;
    }

    // mpi variables
    int rank,  size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // graph variables
    Graph *graph = NULL; // matrix representing the graph
    int n; // number of vertex

    if (rank == 0) {
        char *filename = argv[1]; // file containing the graph

        //////////////////////
        // initializing graph
        //////////////////////
        graph = read_file(filename);
        if (!graph) {
            printf("Error reading graph\n");
            MPI_Abort(MPI_COMM_WORLD, ERROR_READING_GRAPH);
            return 0;
        }

        ////////////////////////////////
        // broadcast n to other threads
        ////////////////////////////////
        n = graph->n;
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD); // 1 sync

        //////////////////////////////////////////////
        // Assigning Random Numbers to all vertex.
        // It is done in parallel using MPI_Allgatherv
        //////////////////////////////////////////////
        srand(time(NULL)); // seed of rand
        double initial = MPI_Wtime();

        int rank0_part_n = n / size + n % size;
        int part_n = n / size;
        int w_send[part_n]; // Starting address of send buffer
        int w_recv[n]; // Starting address of recv buffer
        int recvcounts[size]; // number of elements that are received from each process
        int displs[size]; // displacement (relative to recvbuf) at which to place the incoming data from process i

        recvcounts[0] = rank0_part_n;
        for (int i = 1; i < size; i++) recvcounts[i] = part_n;

        for (int i = 0; i < rank0_part_n; i++) w_send[i] = rand() % 1000; //w_send[i] = 100 + (rand() % 1000);

        displs[0] = 0;
        for (int i = 1; i < size; i++) displs[i] = ((i-1) * part_n) + rank0_part_n;

        MPI_Allgatherv(w_send, rank0_part_n, MPI_INT, w_recv, recvcounts, displs, MPI_INT, MPI_COMM_WORLD);

        // printf("w = ");
        // for (int i = 0; i < n; i++) {
        //     printf("%d ", w_recv[i]);
        // }
        // printf("\n");

        ////////////////////////////////////////////////////////////////
        // Process 0 manages the variables used in the coloring process
        ////////////////////////////////////////////////////////////////
        int color[n];
        for(int i = 0; i < n; i++){
            color[i] = -1;
        }
        int finished_counter = 0;
        int **resp;
        int header[2];

        while (finished_counter != size-1) { // coloring loop
            MPI_Recv(header, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == TAG_GET_N_V) {
                resp = get_uncolored_neighbours(graph, header[0], color);
                
                // Send number of neighbours
                int size = *resp[0];
                MPI_Send(resp[0], 1, MPI_INT, status.MPI_SOURCE, TAG_GET_N_V, MPI_COMM_WORLD);

                // Send neighbours
                if (*resp[0]) {
                    MPI_Send(resp[1], *resp[0], MPI_INT, status.MPI_SOURCE, TAG_GET_N_V, MPI_COMM_WORLD);
                }

                free(resp[0]);
                free(resp[1]);
                free(resp);
            }
            else if (status.MPI_TAG == TAG_COLORS_N_V) {
                // header = [vertex, NULL]
                resp = get_neighbours_color(graph, header[0], color);
                
                // Send number of colors
                MPI_Send(resp[0], 1, MPI_INT, status.MPI_SOURCE, TAG_COLORS_N_V, MPI_COMM_WORLD);

                // Send neighbours colors
                if (*resp[0]) {
                    MPI_Send(resp[1], *resp[0], MPI_INT, status.MPI_SOURCE, TAG_COLORS_N_V, MPI_COMM_WORLD);
                }

                free(resp[0]);
                free(resp[1]);
                free(resp);
            }
            else if (status.MPI_TAG == TAG_COLOR_VI) {
                // header = [vertex, color]
                color[header[0]] = header[1];
            }
            else if (status.MPI_TAG == TAG_FINESHED) {
                finished_counter++;
            }
        }
    
        double final = MPI_Wtime();
        double result = final-initial;

        //MPI_Abort(MPI_COMM_WORLD, ERROR_READING_GRAPH);
        printf("Colors: ");
        for(int i = 0; i < n; i++){
            printf("%d ", color[i]);
        }
        printf("\n");
        printf("Calculation Execution Time: %lf\n", result);

        graph_destroy(graph);
    }
    else {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD); // 1 sync

        //////////////////////////////////////////////
        // Assigning Random Numbers to all vertex.
        // It is done in parallel using MPI_Allgatherv
        //////////////////////////////////////////////
        srand(time(NULL) + rank); // seed of rand

        int rank0_part_n = n / size + n % size;
        int part_n = n / size;
        int w_send[part_n];
        int w_recv[n];
        int recvcounts[size];
        int displs[size];

        recvcounts[0] = rank0_part_n;
        for (int i = 1; i < size; i++) recvcounts[i] = part_n;

        for (int i = 0; i < part_n; i++) w_send[i] = rand() % 1000; //w_send[i] = rand() % 99;

        displs[0] = 0;
        for (int i = 1; i < size; i++) displs[i] = ((i-1) * part_n) + rank0_part_n;

        MPI_Allgatherv(w_send, part_n, MPI_INT, w_recv, recvcounts, displs, MPI_INT, MPI_COMM_WORLD);

        int rest = 0;
        part_n = n / (size-1);
        if (rank == size-1) rest = n % (size-1);

        /////////////////////////////////////////
        // vertex to be colored by process "rank"
        /////////////////////////////////////////
        int set_I[part_n]; // independent set I
        int header[2];
        //int colored[part_n+rest];
        //for (int i = 0; i < part_n+rest; i++) colored[i] = 0;
        int *colored = calloc(part_n+rest, sizeof(int));

        while (1) {
            int count = -1;
            int size_I = 0;
            for (int v = (rank-1)*part_n; v < (rank-1)*part_n + part_n + rest; v++) {
                count++;
                if (colored[v-((rank-1)*part_n)]) continue;
                ///////////////////////
                // get neighbours of v
                ///////////////////////
                int neighbours_size;
                
                // ask for uncolored neighbours of v
                header[0] = v; // empty header
                header[1] = 0;
                MPI_Send(header, 2, MPI_INT, 0, TAG_GET_N_V, MPI_COMM_WORLD);

                // receive uncolored neighbours size
                MPI_Recv(&neighbours_size, 1, MPI_INT, 0, TAG_GET_N_V, MPI_COMM_WORLD, &status);

                // receive uncolored neighbours of v
                int neighbours[neighbours_size];
                if (neighbours_size) {
                    MPI_Recv(neighbours, neighbours_size, MPI_INT, 0, TAG_GET_N_V, MPI_COMM_WORLD, &status);
                }

                ///////////////////////////
                // build independent set I
                ///////////////////////////
                int add_to_set_i = 1;
                for (int i = 0; i < neighbours_size; i++) {
                    int w = neighbours[i];
                    if (w_recv[v] < w_recv[w]) {
                        add_to_set_i = 0;
                        break;
                    }
                }

                if (add_to_set_i) {
                    set_I[size_I] = v;
                    size_I++;
                }
            }

            for (int i = 0; i < size_I; i++) {
                colored[set_I[i]-((rank-1)*part_n)] = 1;
                int v_i = set_I[i];
                int colors_size;
                
                /////////////////////////
                // get neighbours colors
                /////////////////////////
                header[0] = v_i;
                MPI_Send(header, 2, MPI_INT, 0, TAG_COLORS_N_V, MPI_COMM_WORLD);

                // receive colors size
                MPI_Recv(&colors_size, 1, MPI_INT, 0, TAG_COLORS_N_V, MPI_COMM_WORLD, &status);

                // receive colors of v_i
                int neighbours_colors[colors_size];
                if (colors_size) {
                    MPI_Recv(neighbours_colors, colors_size, MPI_INT, 0, TAG_COLORS_N_V, MPI_COMM_WORLD, &status);
                }

                ////////////////////
                // color vertex v_i
                ////////////////////
                int min_color = 0;
                int prev_min_color = -1;
                while (prev_min_color != min_color) {
                    prev_min_color = min_color;
                    for (int i = 0; i < colors_size; i++) {
                        if (neighbours_colors[i] == min_color) {
                            min_color++;
                            break;
                        }
                    }
                }

                header[0] = v_i; // vertex to be colored
                header[1] = min_color; // color to be used
                
                // Coloring vertex v_i
                MPI_Send(header, 2, MPI_INT, 0, TAG_COLOR_VI, MPI_COMM_WORLD);
            }
                
            int all_colored = 1;
            for (int i = 0; i < part_n+rest; i++) {
                if (!colored[i]) {
                    all_colored = 0;
                    break;
                }
            }

            if (all_colored) {
                MPI_Send(header, 2, MPI_INT, 0, TAG_FINESHED, MPI_COMM_WORLD);
                free(colored);
                break;
            }
        }
    }
   
    MPI_Finalize();
    return 0;
}