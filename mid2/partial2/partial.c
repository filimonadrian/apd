#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <time.h>

#define MASTER 0
#define INTERVAL 10000

int main(int argc, char *argv[])
{
    long interval = 1000000;
    int rank;
    double x, y;
    int circlePoints = 0;
    double z;
    double pi;
    int numtasks;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    int points[numtasks];
    srand(time(NULL));

    if (rank == MASTER) {

        scanf("Interval = %ld\n", &interval);
        for (int i = 0; i < interval; i++) {
            x = ((double)rand()) / RAND_MAX;
            y = ((double)rand()) / RAND_MAX;

            /* check if number is inside circle */            
            z = sqrt(x * x + y * y);
            if (z <= 1) {
                circlePoints++;
            }
        }

        for (int i = 0; i < numtasks; i++) {
            MPI_Send(&interval, 1, MPI_LONG, i, 1, MPI_COMM_WORLD);
            // if (i == 0) {
            //     MPI_Recv(&interval, 1, MPI_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // }
            MPI_Recv(&points[i], numtasks, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        points[0] = circlePoints;

        int finalcount = 0;
        long finalInterval = 0;
        for (int i = 0; i < numtasks; ++i) {
            finalcount += points[i];
        }
        finalcount += circlePoints;
        finalInterval = interval * (numtasks + 1);

        pi = ((double)finalcount / (double)finalInterval) * 4.0;

        printf("Pi: %lf\n", pi);

    } else if (rank != 0) {
        MPI_Recv(&interval, 1, MPI_LONG, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < interval; i++) {
            x = ((double)rand()) / RAND_MAX;
            y = ((double)rand()) / RAND_MAX;

            /* check if number is inside circle */            
            z = sqrt(x * x + y * y);
            if (z <= 1) {
                circlePoints++;
            }
        }

        for (int i = 0; i < numtasks; i++) {
            MPI_Send(&circlePoints, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}