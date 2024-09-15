public class Main {
    public class Main {
    public static void main(String[] args) {
        int N = 10; // Número de threads
        int I = 100; // Número de incrementos

        Counter counter = new Counter();

        // Array para armazenar as threads
        Thread[] threads = new Thread[N];

        // Cria e inicia threads
        for (int i = 0; i < N; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < I; j++) {
                    counter.increment();
                }
            });
            threads[i].start();
        }

        for (Thread thread : threads) {
            try {
                thread.join(); // Espera pelo fim da thread
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Valor final do contador: " + counter.getCount());
    }
}

}
