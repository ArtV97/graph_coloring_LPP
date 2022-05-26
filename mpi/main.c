#include "util.h"
#include "mpi.h"

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

    // mpi variables
    int rank,  size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // graph variables
    int **graph = NULL;
    int n;

    if (rank == 0) {
        char *filename = argv[1];
        graph = read_file(filename, &n);
        if (!graph) {
            printf("Error reading graph\n");
            MPI_Abort(MPI_COMM_WORLD, ERROR_READING_GRAPH);
            return 0;
        }

        // broadcast n to other threads
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // broadcast graph to other threads
        for (int i = 0; i < n; i++){
            MPI_Bcast(graph[i], n, MPI_INT, 0, MPI_COMM_WORLD);
        }
        PE = (int *)calloc(n, sizeof(int));
        PS = (int *)calloc(n, sizeof(int));
        pai = (int *)calloc(n, sizeof(int));
        cor = (int *)calloc(n, sizeof(int));

        // o processo 0 ira gerenciar a busca
        // usar a tag do Send e Recv para descobrir o que fazer

    }
    else {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Rank = %d, n = %d\n", rank, n);
        graph = (int **)calloc(n, sizeof(int *));
        for (int i = 0; i < n; i++) {
            graph[i] = (int *)calloc(n, sizeof(int));
            MPI_Bcast(graph[i], n, MPI_INT, 0, MPI_COMM_WORLD);
        }

        // for (int i = 0; i < n; i++) {
        //     for (int j = 0; j < n; j++) {
        //         printf("graph[%d][%d] = %d\n", i, j, graph[i][j]);
        //     }
        // }

        PE = (int *)calloc(n, sizeof(int));
        PS = (int *)calloc(n, sizeof(int));
        pai = (int *)calloc(n, sizeof(int));
        cor = (int *)calloc(n, sizeof(int));

        // if (!coloring(graph, n, 0, 0)) {
        //     printf("Eh 2-colorivel\n");
        // }
                

        // n,m = input().split()               # ler numero de vertices e arestas
        // n = int(n)
        // m = int(m)
        // n_out = [[] * n for i in range(n)]  # definir listas de adjacencia
        // for j in range(m):                  # ler as m arestas do digrafo
        //     a,b = input().split()           # ler aresta de a para b
        //     a = int(a)
        //     b = int(b)
        //     n_out[a].append(b)          # acrescentar b como vizinho de saida de a
        // entrou = n * [False]
        // saiu = n * [False]
        // ciclo = False                       # inicializar variavel ciclo
        // a = 0
        // while a<n and not ciclo:
        //     if not entrou[a]:
        //         pilha = [a]
        //         while pilha and not ciclo:
        //             v = pilha[-1]
        //             entrou[v] = True
        //             if n_out[v]:
        //                 while n_out[v] and not ciclo:
        //                     w = n_out[v].pop()
        //                     if not entrou[w]:
        //                         pilha.append(w)
        //                         break
        //                     elif not saiu[w]:
        //                         ciclo = True
        //             else:
        //                 saiu[v] = True
        //                 pilha.pop()
        //     a += 1
        // if ciclo:
        //     print('SIM')
        // else:
        //     print('NAO')
        
    }

    free(graph);
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(PE);
    free(PS);
    free(pai);
    free(cor);

    
    MPI_Finalize();
    return 0;
}