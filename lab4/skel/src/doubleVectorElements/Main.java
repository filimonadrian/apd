package doubleVectorElements;
/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	public static void main(String[] args) {
		int N = 100000013;
		int v[] = new int[N];
		int numberOfThreads = Runtime.getRuntime().availableProcessors();
		Thread [] threads = new Thread[numberOfThreads];

		for(int i=0;i<N;i++)
			v[i]=i;
		
		// Parallelize me
//		for (int i = 0; i < N; i++) {
//			v[i] = v[i] * 2;
//		}

		for (int i = 0; i < numberOfThreads; i++) {
			threads[i] = new Thread(new MyTask(i, N, numberOfThreads, v));
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

class MyTask implements Runnable {
	int id;
	int N;
	int numberOfThreads;
	int [] v = new int[N];

	public MyTask(int id, int N, int numberOfThreads, int [] v) {
		this.id = id;
		this.N = N;
		this.numberOfThreads = numberOfThreads;
		this.v = v;
	}

	@Override
	public void run() {
		int start = id * N/numberOfThreads;
		int end = Math.min((id + 1) * N/numberOfThreads, N);

		for (int i = start; i < end; i++) {
			v[i] *= 2;
		}
	}

}
