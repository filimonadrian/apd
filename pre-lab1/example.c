#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)
#define NUM_ITERATIONS 100

void *f(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        printf("Hello World din thread-ul %ld, iteratia %d!\n", id, i);
    }
    pthread_exit(NULL);
}

void *g(void *arg) {
    long id = (long)arg;
    printf("Hello World from g function! Thread %ld\n", id);
    pthread_exit(NULL);
}

void *h(void *arg) {
    long id = (long)arg;
    printf("Hello World from h function! Thread %ld\n", id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // long cores = sysconf(_SC_NPROCESSORS_CONF);
    printf("Sistemul are %ld core-uri!\n", NUM_THREADS);
    pthread_t threads[NUM_THREADS];
    int r;
    long id;
    void *status;

    for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_create(&threads[id], NULL, f, (void *)id);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_join(threads[id], &status);

        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    pthread_t t1, t2;
	id = 1;
    r = pthread_create(&t1, NULL, g, (void *)id);
	if (r) {
		printf("Thread 1 was not created");
	}
	id = 2;
    r = pthread_create(&t2, NULL, h, (void *)id);
	if (r) {
		printf("Thread 2 was not created");
	}
        pthread_exit(NULL);
}
