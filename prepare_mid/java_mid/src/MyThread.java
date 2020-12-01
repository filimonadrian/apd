public class MyThread implements Runnable {
    int id;
    int N;
    int numberOfThreads;
    int[] v = new int[N];

    public MyThread(int id, int N, int numberOfThreads, int[] v) {
        this.id = id;
        this.N = N;
        this.numberOfThreads = numberOfThreads;
        this.v = v;
    }

    @Override
    public void run() {
        int start = id * N / numberOfThreads;
        int end = Math.min((id + 1) * N / numberOfThreads, N);

        for (int i = start; i < end; i++) {
            v[i] *= 2;
        }
    }
}
