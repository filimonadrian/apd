#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)
#define NUM_ITERATIONS 100

void *f(void *arg) {
  	long id = (long)arg;
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		printf("Hello World din thread-ul %ld, iteratia %d!\n", id, i);

	}
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

  	pthread_exit(NULL);
}
