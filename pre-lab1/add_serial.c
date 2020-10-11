#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MIN(a,b) (((a) < (b))?(a):(b))
#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)
#define NUM_ITERATIONS 100
#define DIFF 100

/*
    schelet pentru exercitiul 5
*/

int* arr;
int array_size;

typedef struct range {
    int start;
    int end;
} Range;


void *add(void *arg) {

    Range *r = (Range*)arg;

    int start = r->start;
    int end = r->end;
    
    for (int i = start; i < end; i++) {
        arr[i] += DIFF;
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t threads[NUM_THREADS];

    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
    printf("\n");

    // multi-thread addition
    int r;

    for (int id = 0; id < NUM_THREADS; id++) {
        Range *range;
        range = malloc(sizeof(Range));
        range->start = id * (double)array_size / NUM_THREADS;
        range->end = MIN((id + 1) * (double)array_size / NUM_THREADS, array_size);

        r = pthread_create(&threads[id], NULL, add, (void *)range);
        // free(range);

        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            free(range);
            exit(-1);
        }
    }

    // TODO: aceasta operatie va fi paralelizata
  	// for (int i = 0; i < array_size; i++) {
    //     arr[i] += 100;
    // }



    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

  	pthread_exit(NULL);
}
