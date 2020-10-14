#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)
#define DIFF 100

int *arr;
int array_size;

typedef struct range {
    int start;
    int end;
} Range;

void *add(void *arg) {

    Range *r = (Range *)arg;

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

    // allocate a vector of ranges and send them with every thread
    Range *ranges[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        ranges[i] = malloc(sizeof(Range));
        ranges[i]->start = i * (double)array_size / NUM_THREADS;
        ranges[i]->end = MIN((i + 1) * (double)array_size / NUM_THREADS, array_size);
    }

    // verify range for every thread
    // for (int i = 0; i < NUM_THREADS; i++) {
    //     printf("%d %d\n", ranges[i]->start, ranges[i]->end);
    // }

    for (int id = 0; id < NUM_THREADS; id++) {
        r = pthread_create(&threads[id], NULL, add, (void *)ranges[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            exit(-1);
        }
    }


    // TODO: aceasta operatie va fi paralelizata
    // for (int i = 0; i < array_size; i++) {
    //     arr[i] += 100;
    // }

    void *status;
    for (int id = 0; id < NUM_THREADS; id++) {
        r = pthread_join(threads[id], &status);
        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            exit(-1);
        }
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    free(arr);
    for (int i = 0; i < NUM_THREADS; i++) {
        free(ranges[i]);
    }

    pthread_exit(NULL);

    // exercise 6
    /* 
    -- commented printf and single thread:
    
    time ./add_serial 99999999

    real	0m0,484s
    user	0m0,419s
    sys	0m0,064s

    -- commented printf and multithread

    time ./add_serial 99999999

    real	0m0,227s
    user	0m0,579s
    sys	0m0,085s

    */
}
