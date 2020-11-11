package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;

/**
 * @author Gabriel Gutu <gabriel.gutu at upb.ro>
 *
 */
public class Buffer {
    
    Queue queue;
    int size;
    int currentSize;

    public Buffer(int size) {
        queue = new LimitedQueue(size);
        this.size = size;
        this.currentSize = 0;
    }
    void produce(int value) {
        queue.add(value);
        currentSize++;
    }

    void consume() {
        currentSize--;
        // return (int)queue.poll();
    }

    boolean isFull() {
        return currentSize == size;
    }

	void put(int value) {
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

	int get() {
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
            return (int)queue.poll();
        }
	}
}
