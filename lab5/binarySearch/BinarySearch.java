package binarySearch;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class BinarySearch {
    static CyclicBarrier barrier;

    static void populateArr(int arr[], int value) {
        for (int i = 0; i < value; i++) {
            arr[i] = i;
        }
    }

    public static void main(String[] args) {
        int[] arr = new int[1001];
        int find = 35;
        int P = 8;
        barrier = new CyclicBarrier(P);

        populateArr(arr, 1001);

        Thread[] threads = new Thread[P];
        for (int id = 0; id < P; id++) {
            threads[id] = new Thread(new MyThread(id, arr, P, find));
            threads[id].start();
        }

        for (int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

class MyThread implements Runnable {
    int id;
    int [] arr;
    int P;
    int element;
    static volatile int start, end;
    static volatile boolean found = false;

    public MyThread(int id, int[] arr, int P, int find) {
        this.id = id;
        this.arr = arr;
        this.P = P;
        this.element = find;
        start = 0;
        end = arr.length - 1;
    }

    @Override
    public void run() {
        int threadStart, threadEnd;
        threadStart = id * (end - start) / P + start;
        threadEnd = Math.min((id + 1) * (end - start)/ P + start, end);

        while (threadStart < threadEnd && !found) {
            threadStart = id * (end - start) / P + start;
            threadEnd = Math.min((id + 1) * (end - start)/ P + start, end);
            try {
                BinarySearch.barrier.await();
            } catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
            }

            if (element >= arr[threadStart] && element <= arr[threadEnd]) {

                start = threadStart;
                end = threadEnd;
            }
            if (arr[threadStart] == element) {
                found = true;
                System.out.println("Elementul este" + arr[threadStart]);
            }

            try {
                BinarySearch.barrier.await();
            } catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}