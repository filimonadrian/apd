
public class HelloWorld {

    public static void main(String[] args) {
        int numberOfThreads = Runtime.getRuntime().availableProcessors();
        System.out.println("This computer has " + numberOfThreads + " processors");
        Thread [] threads = new Thread[numberOfThreads];

        for (int i = 0; i < numberOfThreads; i++) {
            threads[i] = new Thread(new MyHelloWorldThreads(i));
            threads[i].start();
        }

        for (int i = 0; i < numberOfThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}

class MyHelloWorldThreads implements Runnable {
    protected int id;

    public MyHelloWorldThreads(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    @Override
    public void run() {
        System.out.println("Hello world from thread " + id + "!");
    }
}