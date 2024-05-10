// As threads vao por cada iteração do ciclo atribuir um valor random para o propose e sera calculado o valor mais escolhido pelas mesmas naquela geração.

import java.util.HashMap;
import java.util.Random;

class Agreement {
    private final int TotalThreads;
    private int CurrentThread = 0;
    private int generation = 0;
    private int max;
    HashMap<Integer,Integer>map = new HashMap<Integer,Integer>();

    public Agreement (int N) {
        this.TotalThreads = N;
    }

    public synchronized int propose(int choice) throws InterruptedException {
        int number = 0;
        int currentGeneration = generation;
        CurrentThread++;
        if(CurrentThread == TotalThreads) {
            CurrentThread = 0;
            generation++;
            for(int i : map.values()) {
                int conta = 0;
                for(int j : map.values()){
                    if(i == j) conta++;
                }
                if (number < conta){
                    number = conta;
                    this.max = i;
                }
            }
            System.out.println("--------------------- <3 Valor eleito: " + this.max + " <3 ----------------------");
            map.clear();
            notifyAll();
        }
        else{
            while (currentGeneration == generation) {
                System.out.println(CurrentThread + " is waiting with value: " + choice);
                map.put(CurrentThread, choice);
                wait();
            }
        }
        return 0;
    }
}
    

public class Main {
    public static void main(String[] args) throws InterruptedException {
        final int N = 10;
        Agreement agreement = new Agreement(N);
        Random number = new Random();
        Runnable runnable = () -> {
            for (int i = 0; i < 5; i++) { // Each thread will wait at the barrier 5 times
                try {
                    int valor = number.nextInt(5);
                    agreement.propose(valor);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        };
        for (int i = 0; i < 10; i++) {
            new Thread(runnable).start();
        }
    }
}
