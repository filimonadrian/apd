// package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	static CyclicBarrier barrier;

	public static void main(String[] args) {
		int M = 9;
		int N = 5;
//		int numberOfThreads = Runtime.getRuntime().availableProcessors();
		int numberOfThreads = 2;

		Thread [] threads = new Thread[numberOfThreads];
		barrier = new CyclicBarrier(numberOfThreads);

		int graph[][] = { { 0, 1, M, M, M },
				          { 1, 0, 1, M, M }, 
				          { M, 1, 0, 1, 1 }, 
				          { M, M, 1, 0, M },
				          { M, M, 1, M, 0 } };
		
		// Parallelize me (You might want to keep the original code in order to compare)
//		for (int k = 0; k < N; k++) {
//			for (int i = 0; i < N; i++) {
//				for (int j = 0; j < N; j++) {
//					graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
//				}
//			}
//		}

		for (int i = 0; i < numberOfThreads; i++) {
			threads[i] = new Thread(new MyThread(N, numberOfThreads, i, graph));
			threads[i].start();
		}

		for (int i = 0; i < numberOfThreads; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}


		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}
	}
}

class MyThread implements Runnable {
	int N, numberOfThreads;
	int id;
	int graph[][];

	public MyThread(int n, int numberOfThreads, int id, int[][] graph) {
		N = n;
		this.numberOfThreads = numberOfThreads;
		this.id = id;
		this.graph = graph;
	}

	@Override
	public void run() {

		for (int k = 0; k < N; k++) {
			int start = id * N/numberOfThreads;
			int end = Math.min((id + 1) * N/numberOfThreads, N);
			for (int i = start; i < end; i++) {
				for (int j = 0; j < N; j++) {
					graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
				}
			}

			try {
				Main.barrier.await();
			} catch(BrokenBarrierException | InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
