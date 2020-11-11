package oneProducerOneConsumer;
/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	int a = -1;
	int size = 0;

	void produce(int value) {
		a = value;
		size = 1;
	}

	void consume() {
		size = 0;
	}

	boolean isFull() {
		return size == 1;
	}

	void put (int value) {
		synchronized (this) {
			while (isFull()) {
				try {
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			produce(value);
			notify();
		}
	}

	int get () {
		synchronized (this) {
			while (!isFull()) {
				try {
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			consume();
			notify();
			return a;
		}
	}
}
