#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define GROUP_SIZE 4
#define ROOT 0

int main (int argc, char *argv[])
{
    int old_size, new_size;
    int old_rank, new_rank;
    int recv_rank = 0;
    MPI_Status status;    
    MPI_Comm custom_group;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &old_size); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &old_rank); // The current process ID / Rank.

    int ret = 0;

    if (old_size % GROUP_SIZE != 0) {
      printf("Incorrect number of processes. Must be a multiple of %d", GROUP_SIZE);
      exit(1);
    } 

    // Split the MPI_COMM_WORLD in small groups.
    for (int i = 0; i < old_size; i++) {
      // color == new id of the group
      // key == id of the processes in this group
      ret = MPI_Comm_split(MPI_COMM_WORLD, old_rank / GROUP_SIZE, old_rank % GROUP_SIZE, &custom_group);
      if (ret != 0) {
        printf("Cannot split the group\n");
      }

      MPI_Comm_size(custom_group, &new_size);
      MPI_Comm_rank(custom_group, &new_rank);
    }


    printf("Rank [%d] / size [%d] in MPI_COMM_WORLD and rank [%d] / size [%d] in custom group.\n",
            old_rank, old_size, new_rank, new_size);

    // Send the rank to the next process.
    // First process starts the circle.
    if (new_rank == ROOT) {
        // Send rank to the next process
        ret = MPI_Send(&new_rank, 1, MPI_INT, (new_rank + 1) % GROUP_SIZE, 0, custom_group);
        if (ret != 0) {
          printf("Can't send data. I am process %d", new_rank);
        }

        ret = MPI_Recv(&recv_rank, 1, MPI_INT, GROUP_SIZE - 1, 0, custom_group, &status);
        if (ret != 0) {
          printf("Can't receive data. I am process %d", new_rank);
        }


    } else if (new_rank == new_size - 1) {
        // Last process. close the circle.
        // Receives the number from the previous process.
        ret = MPI_Recv(&recv_rank, 1, MPI_INT, (new_rank - 1) % GROUP_SIZE, 0, custom_group, &status);
        if (ret != 0) {
          printf("Can't receive data. I am process %d", new_rank);
        }

        // Receive the rank.
        printf("Process [%d] from group [%d] received [%d].\n", new_rank,
            old_rank / GROUP_SIZE, recv_rank);

        // Sends the rank to the first process.
        ret = MPI_Send(&new_rank, 1, MPI_INT, ROOT, 0, custom_group);
        if (ret != 0) {
          printf("Can't send data. I am process %d", new_rank);
        }

    } else {
        // Middle process.
        // Receives the number from the previous process.
        ret = MPI_Recv(&recv_rank, 1, MPI_INT, (new_rank - 1) % GROUP_SIZE, 0, custom_group, &status);
        if (ret != 0) {
          printf("Can't receive data. I am process %d", new_rank);
        }

        // Receive the rank.
        printf("Process [%d] from group [%d] received [%d].\n", new_rank,
            old_rank / GROUP_SIZE, recv_rank);


        // Sends the new_rank to the next process.
        ret = MPI_Send(&new_rank, 1, MPI_INT, (new_rank + 1) % 4, 0, custom_group);
        if (ret != 0) {
          printf("Can't send data. I am process %d", new_rank);
        }
    }

    MPI_Finalize();
}

