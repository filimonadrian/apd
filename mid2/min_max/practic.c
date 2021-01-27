#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define N 100

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

int main (int argc, char *argv[])
{
    int nProcesses, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int min, max;
    int dim = N / nProcesses;

    int *v_recv = (int*)malloc(sizeof(int) * dim);

    if(rank == MASTER){
    	int *v = (int*)malloc(sizeof(int) * N);

    	srandom(42);
		for(int i = 0; i < N; i++)
			v[i] = random() % 200;
		displayVector(v);

		for(int i = 0; i < nProcesses; i ++)
			MPI_Send(&v[dim*i], dim, MPI_INT, i, 0, MPI_COMM_WORLD);

		MPI_Recv(v_recv, dim, MPI_INT,MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//printf("Process [%d]: have elements %d %d.\n", rank, v_recv[0], v_recv[1]);

		min = v_recv[0];
		max = v_recv[0];

		for(int i = 0; i < dim; i ++){
			if(v_recv[i] > max) max = v_recv[i];

			if(v_recv[i] < min) min = v_recv[i];
		}

		//printf("Process [%d]: has min: %d and max: %d.\n", rank, min, max);
    }

    else{
    	MPI_Recv(v_recv, dim, MPI_INT,MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	//printf("Process [%d]: have elements %d %d.\n", rank, v_recv[0], v_recv[1]);

    	min = v_recv[0];
		max = v_recv[0];

		for(int i = 0; i < dim; i ++){
			if(v_recv[i] > max) max = v_recv[i];

			if(v_recv[i] < min) min = v_recv[i];
		}

		//printf("Process [%d]: has min: %d and max: %d.\n", rank, min, max);

		MPI_Send(&min, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
		MPI_Send(&max, 1, MPI_INT, MASTER, 2, MPI_COMM_WORLD);
    }

    if(rank == MASTER){
    	int min_recv, max_recv;

    	for(int i = 1; i < nProcesses; i++){
    		MPI_Recv(&min_recv, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		MPI_Recv(&max_recv, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    		if(min_recv < min) min = min_recv;
    		if(max_recv > max) max = max_recv;
    	}

    	printf("Min is %d and max is %d\n", min, max);
    }

    MPI_Finalize();

}

