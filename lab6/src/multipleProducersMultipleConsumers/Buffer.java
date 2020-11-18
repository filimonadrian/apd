package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer {
	int value;
	ArrayBlockingQueue<Integer> arr = new ArrayBlockingQueue<Integer>(1);

	void put(int value) {
		try {
			this.arr.put(value);
		} catch (InterruptedException ex) {
			ex.printStackTrace();
		}
	}

	int get() {
		try {
			return this.arr.take();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		return 0;
	}
}
