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

void *add(void *arg) {

    int id = (long)arg;
    int start = id * (double)array_size / NUM_THREADS;
    int end = MIN((id + 1) * (double)array_size / NUM_THREADS, array_size);
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
    long id = 0;
    for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_create(&threads[id], NULL, add, (void *)id);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
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

    pthread_exit(NULL);
}
