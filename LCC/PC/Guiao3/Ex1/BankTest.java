import java.util.Random;

public class BankTest {

    private static class Mover implements Runnable {
        private Bank b;
        private int novas_accs;
        private String flag;

        public Mover(Bank b, int accs, String flag) {
            this.b = b;
            this.novas_accs = accs;
            this.flag = flag;
        }


        public void run() {
            for (int m = 0; m < novas_accs; m++) {
                int a = b.createAccount(1000);
                System.out.println("Conta " + a + " criada. " + flag);
            }
        }
    }

    private static class Transf implements Runnable {
        private Bank b;
        private int accs;
        private int value;

        public Transf(Bank b, int accs, int value) {
            this.b = b;
            this.accs = accs;
            this.value = value;
        }


        public void run() {
            Random rand = new Random();
            Random valor = new Random();
            for (int m = 0; m < accs; m++) {
                int dest = rand.nextInt(accs);
                int trans = valor.nextInt(value);
                b.transfer(m, dest, trans);
                System.out.println("Transferir:" + trans + " Origem " + m + " Value:" + b.balance(m) + " Destino " + dest + " Value:" + b.balance(dest));
            }
        }
    }

    private static class Close implements Runnable {
        private Bank b;
        private int accs;

        public Close(Bank b, int accs) {
            this.b = b;
            this.accs = accs;
        }


        public void run() {
            int valor_closed = 0;
            Random rand = new Random();
            for (int m = 0; m < accs; m++) {
                int id = rand.nextInt(accs);
                int x = b.closeAccount(id);
                if (x == -1) {
                    System.out.println("A conta " + id + " nÃ£o existe!");
                }
                else{  
                    valor_closed += x;
                    System.out.println("Closed -> Id: " + id + " Value: " + x);
                }  
            }
        }
    }

    private static class Observer implements Runnable {
        private Bank b;
        private int accs;

        public Observer(Bank b, int accs) {
            this.b = b;
            this.accs = accs;
        }


        public void run() {
            for (int i : b.getMap().keySet()) {
                int balance = b.balance();
                System.out.println("balance: " + balance);
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        int ACCS = 10;
        Bank b = new Bank();

        Thread t1 = new Thread(new Mover(b, ACCS, "t1")); 
        Thread t2 = new Thread(new Mover(b, ACCS ,"t2"));
        Thread t3 = new Thread(new Transf(b, ACCS * 2, 1000));
        Thread t4 = new Thread(new Close(b, ACCS));
        Thread t5 = new Thread(new Observer(b, ACCS * 2));
        t5.start();
        t1.start();
        t2.start();
        t3.start();
        t4.start();

        t1.join();
        t2.join();
        /*for (int a : b.getMap().keySet()) {
            System.out.println("Id: " + a + " Balance: " + b.balance(a));
        }*/
        t3.join();
        t4.join();
        t5.join();
        System.out.println(b.totalBalance());
    }

}