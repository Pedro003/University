class Barrier {
    private final int totalThreads;
    private int waitingThreads = 0;
    private int generation = 0;
    private final Object lock = new Object();

    public Barrier(int totalThreads) {
        this.totalThreads = totalThreads;
    }

    public void await() throws InterruptedException {
        synchronized (lock) {
            int currentGeneration = generation;
            waitingThreads++;
            if (waitingThreads == totalThreads) {
                waitingThreads = 0;
                generation++;
                lock.notifyAll();
            } else {
                while (currentGeneration == generation) {
                    lock.wait();
                }
            }
        }
    }
}

public class Main {
    public static void main(String[] args) {
        int N = 10;
        Barrier barrier = new Barrier(N);

        Runnable runnable = () -> {
            for (int i = 0; i < 5; i++) { // Each thread will wait at the barrier 5 times
                System.out.println(Thread.currentThread().getName() + " is waiting");
                try {
                    barrier.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                System.out.println(Thread.currentThread().getName() + " is released");
            }
        };

        for (int i = 0; i < N; i++) {
            new Thread(runnable).start();
        }
    }
}