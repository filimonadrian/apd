#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int recv_num;
    int random_num, ret = 0, inc_val = 2;
    MPI_Status status;
    srand(30);

    int arr[numtasks];
    int sol_arr[2 * numtasks];

    for (int i = 0; i < numtasks; i++) {
        arr[i] = i;
    }

    /* fiecare proces cunoaste vectorul initial
     * deci in afara de primul proces, care nu primeste nimic
     * primul proces trimite 2 valori(a dublat-o pe a doua)
     * celelalte procese o sa primeasca process * 2 valori(daca e proceosul 3, inseamna ca proceseaza arr[2]
     * si trebuie sa primeasca 4 valori de la procesul de dinaintea lui) 
     * 
    */
    // First process starts the circle.
    if (rank == MASTER) {
        
        /* procesul meu are id-ul 0, deci procesez primul element
         * mai precis il trimit de 2 ori la urmatorul proces
         * 
         */

        MPI_Send(arr, 2, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);

    /* daca sunt ultimul proces, inseamna ca trebuie sa primesc toate datele */
    } else if (rank == numtasks - 1) {
        // Last process close the circle.

        ret = MPI_Recv(sol_arr, rank * 2, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &status);
        if (ret != 0) {
            printf("Can't receive data. I am process %d", rank);
        }
        for (int i = 0; i < rank * 2; i++) {
            printf(" Proc %d: Received %d\n", rank, sol_arr[i]);
        }


        sol_arr[rank * 2] = arr[rank];
        sol_arr[rank * 2 + 1] = arr[rank];

        /* afiseaza rezultatul */
        printf("Rezultatul este: ");
        for (int i = 0; i < numtasks * 2; i++) {
            printf("%d ", sol_arr[i]);
        }

    } else {
        // Middle process.
        
        /* procesele din mijloc primesc toate datele, le pun in array-ul de solutii
         * isi proceseaza elementul, il adauga si ele in array-ul de solutii
         * apoi trimit tot arrray-ul mai departe
         * 
         * 
         */
        
        int recv_value = 0;
        /* daca sunt procesul 3 sa zicem, eu trebuie sa primesc 6 elemente 
         * de la 0 0 , 1 1, 2 3 */

        ret = MPI_Recv(sol_arr, rank * 2, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &status);
        if (ret != 0) {
            printf("Can't receive data. I am process %d", rank);
        }

        for (int i = 0; i < rank * 2; i++) {
            printf(" Proc %d: Received %d\n", rank, sol_arr[i]);
        }

        /* dupa ce am primit toate numerele, imi calculez partea */
        sol_arr[rank * 2] = arr[rank];
        sol_arr[rank * 2 + 1] = arr[rank];

        /* trimit toate datele mai departe
         * +2 pentru ca am adaugat si noile valori
         * */
        
        ret = MPI_Send(sol_arr, rank * 2 + 2, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
        if (ret != 0) {
            printf("Can't receive data. I am process %d", rank);
        }

        for (int i = 0; i < rank * 2 + 2; i++) {
            printf(" Proc %d: Send %d\n", rank, sol_arr[i]);
        }
    }

    MPI_Finalize();

}

