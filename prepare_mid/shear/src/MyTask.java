import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

class MyTask implements Runnable {
    int id, N, L;
    int numberOfThreads;
    Integer [][] M;
    public MyTask(int id, int N, int L, int numberOfThreads, Integer [][] M) {
        this.id = id;
        this.N = N;
        this.L = L;
        this.numberOfThreads = numberOfThreads;
        this.M = M;
    }

    @Override
    public void run() {
        int start = id * N/numberOfThreads;
        int end = Math.min((id + 1) * N/numberOfThreads, N);
        int k = 0, i = 0, j = 0;
        int [] aux = new int [L];

        for (k = 0; k < Math.log(N) + 1; k++) {
            // se sorteaza liniile pare crescator
            // se sorteaza liniile impare descrescator
            for (i = start; i < end; i++) {
                if (i % 2 != 0) {
                    Arrays.sort(M[i], Collections.reverseOrder());
                } else {
                    Arrays.sort(M[i]);
                }
            }

            // asteapta ca toate threadurile sa termine sortarea
            try {
                Main.barrier.await();
            } catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
            }

            for (i = start; i < end; i++) {
                for (j = 0; j < L; j++) {
                    aux[j] = M[j][i];
                }

                Arrays.sort(aux);

                for (j = 0; j < L; j++) {
                    M[j][i] = aux[j];
                }
            }

            // asteapta ca toate threadurile sa termine sortarea
            try {
                Main.barrier.await();
            } catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
            }

            }
        }
    }
