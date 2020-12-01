#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int P = 0;

// pthread_mutex_lock(&mutex);
// pthread_mutex_unlock(&mutex);
pthread_mutex_t mutex;
 
// pthread_barrier_wait(&barrier);
pthread_barrier_t barrier;

/* struct for thread arguments */
typedef struct _thread_args {
  long id; 

} thread_args;

/* input arguments */
void get_args(int argc, char **argv) {
  /* argv[0] == filename */ 
  /* initialize with some defaults */
  if (argc == 0) {
    P = sysconf(_SC_NPROCESSORS_CONF);
    return; 
  }

	P = atoi(argv[1]);
}

void free_args(thread_args **args) {
	/* Clear all */
  for (long id = 0; id < P; id++) {
		free (args[id]);
	}
  free(args);
}

void destroy_pthread_obj() {
  pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);
}

/* Problem size */
int N = 8;
int arr[8] = {1, 2, 3, 4, 5, 6, 7, 9};

/*!!! Result !!! */
long sum = 0;


void *thread_func(void *arg) {
	thread_args *args = (thread_args *) arg;
	/* Get arguments */
	int id = (int)args->id;
	
	/* Compute start and end */
	long start = id * (double) N / P;
  long end = MIN((id + 1) * (double) N / P, N);

	// printf ("Hello from thread %d! Start: %d. End: %d.\n", id, start, end);
	for (int i = start; i < end; i++) {
	  sum += arr[i];
	}

	/* Done */
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  void *status;
	int ret = 0;
 
  /* read the arguments of the program */
  get_args(argc, argv);
  
  pthread_t threads[P];

  /* Parameters for thread function. */
	thread_args **args = (thread_args **) malloc (P * sizeof (struct _thread_args));
	
	/* Init the mutex and barier */
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, P);

	/* Start threads */
  for (long id = 0; id < P; id++) {
		args[id] = (thread_args *) malloc (sizeof (struct _thread_args));
		args[id]->id = id;
		ret = pthread_create(&threads[id], NULL, thread_func, args[id]);
    if (ret) {
      printf("Can't create thread %ld \n", id);
      exit(1);
      }
  }

	/* Join Threads */
  for (long id = 0; id < P; id++) {
    ret = pthread_join(threads[id], &status);
    if (ret) {
      printf("Can't wait thread %ld \n", id);
      exit(1);
      }
  }

  /* print result */
	printf ("Sum arr: %ld\n", sum);
	printf ("Sum: %ld\n", (long)((2 + N) * (N + 1) / 2));
	printf ("Missing: %d\n", abs(sum - (long)((2 + N) * (N + 1) / 2)));


  free_args(args);
  destroy_pthread_obj();

  return 0;
}
