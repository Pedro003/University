class Barrier {
    private final int totalThreads;
    private int waitingThreads = 0;
    //private final Object lock = new Object();

    public Barrier(int totalThreads) {
        this.totalThreads = totalThreads;
    }

    public synchronized void await() throws InterruptedException {
            waitingThreads++;
            if (waitingThreads == totalThreads) {
                notifyAll();
            } else {
                while (waitingThreads < totalThreads) {
                    wait();
                }
            }
    }
}


public class Main {
    public static void main(String[] args) throws InterruptedException {
        int N = 10;
        Barrier barrier = new Barrier(N);

        Runnable runnable = () -> {
            System.out.println("Thread " + Thread.currentThread().getId() + " is waiting");
            try {
                barrier.await();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            System.out.println("Thread " + Thread.currentThread().getId() + " is released");
        };

        for (int i = 0; i < N; i++) {
            new Thread(runnable).start();
        }
    }
}