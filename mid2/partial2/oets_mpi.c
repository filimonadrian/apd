#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int generate_number(int numtasks, int rank) {
    return (2 * numtasks) - (2 * rank);
}

int main (int argc, char *argv[])
{
    int  numtasks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int number;
    int random_num, ret = 0, inc_val = 2;
    MPI_Status status;
    srand(30);

    number = generate_number(numtasks, rank);
    int neigh_value = 0;

    for (int i = 0; i < numtasks; i++) {
        // if even
        if (i % 2 == 0) {

            // my rank is even ==> wait for right neighbour to send his value
            if (rank % 2 == 0) {
                // if the number of tasks is odd, last rank will be even => no one can send to me
                if ((numtasks - 1) % 2 != 0 || ((numtasks - 1) % 2 == 0 && rank != numtasks - 1)) {
                    // wait for neigh value
                    ret = MPI_Recv(&neigh_value, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD, &status);
                    if (ret != 0) {
                        printf("Can't send data. I am process %d", rank);
                    }

                    // if his value is lower, exchange data
                    if (neigh_value < number) {

                        ret = MPI_Send(&number, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
                        if (ret != 0) {
                            printf("Can't send data. I am process %d", rank);
                        }
                        number = neigh_value;
                    } else {
                        ret = MPI_Send(&neigh_value, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
                        if (ret != 0) {
                            printf("Can't send data. I am process %d", rank);
                        }
                    }
                }

            // rank is odd ==> send to left neigh my number
            } else {
                // send to left neigh my number
                ret = MPI_Send(&number, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD);
                if (ret != 0) {
                    printf("Can't send data. I am process %d", rank);
                }
                // wait for value from neighbour
                ret = MPI_Recv(&number, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &status);
                if (ret != 0) {
                    printf("Can't send data. I am process %d", rank);
                }
            }
        // if odd
        } else {
            // my rank is odd ==> wait for right neighbour to send his value
            if (rank % 2 != 0) {
                if ((numtasks - 1) % 2 == 0 || ((numtasks - 1) % 2 != 0 && rank != numtasks - 1)) {
                    // wait for neigh value
                    ret = MPI_Recv(&neigh_value, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD, &status);
                    if (ret != 0) {
                        printf("Can't send data. I am process %d", rank);
                    }
                    // if his value is lower, exchange data
                    if (neigh_value < number) {
                        ret = MPI_Send(&number, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
                        if (ret != 0) {
                            printf("Can't send data. I am process %d", rank);
                        }
                        number = neigh_value;
                    }
                }
            // rank is even ==> send to left neigh my number
            } else {
                if (rank != 0) {
                    // send to left neigh my number
                    ret = MPI_Send(&number, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD);
                    if (ret != 0) {
                        printf("Can't send data. I am process %d", rank);
                    }
                
                    // wait for value from neighbour
                    ret = MPI_Recv(&number, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &status);
                    if (ret != 0) {
                        printf("Can't send data. I am process %d", rank);
                    }
                }
            }
        }
    }

    for (int i = 0; i < numtasks; i++) {
        if (rank == i) {
            printf("rank: %d, number: %d\n", rank, number);
        }
    }


    MPI_Finalize();

}
