#include <time.h>
#include "mpi.h"
#include "util.h"

#define ERROR_READING_GRAPH 1


#define TAG_NOT_2_COLORING -1
#define TAG_ATT_PE          0
#define TAG_ATT_PS          1
#define TAG_ATT_PAI         2
#define TAG_ATT_COR         3


int *PE = NULL;
int *PS = NULL;
int *pai = NULL;
int *cor = NULL;

int coloring(int **graph, int n, int v, int t) {
    t++;
    // atualiza o PE na posicao v
    PE[v] = t;

    for (int w = 0; w < n; w++) {
        if (!graph[v][w]) continue;

        if (PE[w] == 0) {
            // atualiza o pai na posicao w
            pai[w] = v;

            // atualiza a cor na posicao w
            cor[w] = 1 - cor[v];

            if (coloring(graph, n, w, t)) return 1;
        }
        else if (PS[w] == 0 && w != pai[v]) {
            if (cor[w] != cor[v]) continue;
            else {
                fprintf(stderr, "Nao eh 2-colorivel\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
                return 1;
            }
        }
    }
    t++;
    // atualiza o PS na posicao v
    PS[v] = t;
    return 0;
}

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
    int **graph = NULL; // matrix representing the graph
    int n; // number of vertex

    if (rank == 0) {
        char *filename = argv[1]; // file containing the graph

        //////////////////////
        // initializing graph
        //////////////////////
        graph = read_file(filename, &n);
        if (!graph) {
            printf("Error reading graph\n");
            MPI_Abort(MPI_COMM_WORLD, ERROR_READING_GRAPH);
            return 0;
        }

        ////////////////////////////////
        // broadcast n to other threads
        ////////////////////////////////
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        ////////////////////////////////////
        // broadcast graph to other threads
        ////////////////////////////////////
        for (int i = 0; i < n; i++){
            MPI_Bcast(graph[i], n, MPI_INT, 0, MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD); // 1 sync

        //////////////////////////////////////////////
        // Assigning Random Numbers to all vertex.
        // It is done in parallel using MPI_Allgatherv
        //////////////////////////////////////////////
        srand(time(NULL)); // seed of rand

        int rank0_part_n = n / size + n % size;
        int part_n = n / size;
        int w_send[part_n]; // Starting address of send buffer
        int w_recv[n]; // Starting address of recv buffer
        int recvcounts[size]; // number of elements that are received from each process
        int displs[size]; // displacement (relative to recvbuf) at which to place the incoming data from process i

        recvcounts[0] = rank0_part_n;
        for (int i = 1; i < size; i++) recvcounts[i] = part_n;

        for (int i = 0; i < rank0_part_n; i++) w_send[i] = rand(); //w_send[i] = 100 + (rand() % 1000);

        displs[0] = 0;
        for (int i = 1; i < size; i++) displs[i] = ((i-1) * part_n) + rank0_part_n;

        MPI_Allgatherv(w_send, rank0_part_n, MPI_INT, w_recv, recvcounts, displs, MPI_INT, MPI_COMM_WORLD);

    }
    else {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Rank = %d, n = %d\n", rank, n);
        graph = (int **)calloc(n, sizeof(int *));
        for (int i = 0; i < n; i++) {
            graph[i] = (int *)calloc(n, sizeof(int));
            MPI_Bcast(graph[i], n, MPI_INT, 0, MPI_COMM_WORLD);
        }

        MPI_Barrier(MPI_COMM_WORLD); // 1 sync

        // for (int i = 0; i < n; i++) {
        //     for (int j = 0; j < n; j++) printf("graph[%d][%d] = %d\n", i, j, graph[i][j]);
        // }

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

        for (int i = 0; i < part_n; i++) w_send[i] = rand(); //w_send[i] = rand() % 99;

        displs[0] = 0;
        for (int i = 1; i < size; i++) displs[i] = ((i-1) * part_n) + rank0_part_n;

        MPI_Allgatherv(w_send, part_n, MPI_INT, w_recv, recvcounts, displs, MPI_INT, MPI_COMM_WORLD);

        // printf("Process %d\n", rank);
        // for (int i = 0; i < n; i++) printf("w[%d] = %d ", i, w_recv[i]);
        // printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);
    
    MPI_Finalize();
    return 0;
}