#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int random_num, ret = 0;

    srand(39);

    if (rank == ROOT) {
      // generate a number
      random_num = rand() % 100;
      // print the generated number
      printf("Generated number is %d\n", random_num);
      // send number to all processes
      ret = MPI_Bcast(&random_num, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
      if (ret != 0) {
        printf("Process %d can't broadcast\n", rank);
      }
    }
  
    // Root process generates a random number.
    // Broadcasts to all processes.
    ret = MPI_Bcast(&random_num, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    printf("Process [%d], after broadcast %d.\n", rank, random_num);



    MPI_Finalize();

}

