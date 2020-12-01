import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

// nu functioneaza trecerea din matrice in vector
// nu am veridicat scalabilitatea

public class Main {
    static CyclicBarrier barrier;

    static void printMatrix(Integer [][] M, int N, int L) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < L; j++) {
                System.out.print(M[i][j] + " ");
            }
            System.out.print("\n");
        }
    }

    static void copy_matrix_in_vector(Integer [][] M, Integer [] v, int N, int L) {
        int i, j;
        for (i = 0; i < L; i++) {
            if (i % 2 == 0) {
                for (j = 0; j < L; j++) {
                    v[i * L + j] = M[i][j];
                }
            } else {
                for (j = L; j > 0; j--) {
                    v[i * L + (L - j)] = M[i][j - 1];
                }
            }
        }
    }

    static void display_vector(Integer [] v, int N, int L) {
        int i;

        for (i = 0; i < v.length; i++) {
            System.out.print(v[i] + " ");
        }

        System.out.println("");
    }
    public static void main(String[] args) {
        int N = 4;
        int L = 4;
        int numberOfThreads = 4;
        Integer [][] M = { { 39, 27, 11, 42 },
                            { 10, 93, 91, 90 },
                            { 54, 78, 56, 89 },
                            { 24, 64, 20, 65 } };

        barrier = new CyclicBarrier(numberOfThreads);

        Thread [] threads = new Thread[numberOfThreads];

        for (int i = 0; i < numberOfThreads; i++) {
            threads[i] = new Thread(new MyTask(i, N, L, numberOfThreads, M));
            threads[i].start();
        }

        printMatrix(M, N, L);


        // Integer [] v = new Integer[N + L];
        // copy_matrix_in_vector(M, v, N, L);
        // display_vector(v, N, L);



    }

}