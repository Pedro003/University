class Incrementer extends Thread {
    private int I;

    public Incrementer(int I) {
        this.I = I;
    }

    public void run() {
        for (int i = 1; i <= I; i++) {
            System.out.println(Thread.currentThread().getName() + " : " + i);
        }
    }
}

public class ex1 {
    public static void main(String[] args) {

        int N = Integer.parseInt(args[0]); // Numero de threads
        int I = Integer.parseInt(args[1]); // Numero de iteracoes

        Thread[] a = new Thread[N];

        for (int i = 0; i < N; i++) {
            a[i] = new Incrementer(I);
            a[i].start();
        }

        try {
            for (int i = 0; i < N; i++) {
                a[i].join();
            }
        } catch (InterruptedException e) {
            System.out.println("Interrupted");
        }
    }
}
