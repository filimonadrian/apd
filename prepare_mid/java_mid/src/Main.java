public class Main {

    public static void main(String[] args) {
        int N = 100000013;
        int v[] = new int[N];
        int numberOfThreads = Runtime.getRuntime().availableProcessors();
        Thread [] threads = new Thread[numberOfThreads];

        for(int i = 0;i<N;i++)
            v[i] = i;

        for (int i = 0; i < numberOfThreads; i++) {
            threads[i] = new Thread(new MyThread(i, N, numberOfThreads, v));
            threads[i].start();
        }

        for (int i = 0; i < numberOfThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < N; i++) {
            if(v[i]!= i*2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }

}