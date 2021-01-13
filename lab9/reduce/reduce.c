#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int value = rank;
    int recv_value = 0;
    int sum = value;
    int ret = 0;
    MPI_Status status;

    for (int i = 2; i <= procs; i *= 2) {
        // TODO
        if (rank % i == 0) {
            ret = MPI_Recv(&recv_value, 1, MPI_INT, (rank + (i / 2)), 0, MPI_COMM_WORLD, &status);
            sum += recv_value;
            // printf("pas %d: %d primeste de la %d, sum=%d\n", i, rank, (rank + (i / 2)), sum);

        } else if (rank % (i / 2) == 0) {
            // printf("pas %d: %d ii trimite lui %d, sum=%d\n", i, rank, (rank - (i / 2)), sum);
            ret = MPI_Send(&sum, 1, MPI_INT, (rank - (i / 2)), 0, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        printf("Result = %d\n", sum);
    }

    MPI_Finalize();

}

