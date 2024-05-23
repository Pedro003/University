import java.util.Random;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Adivinha implements Partida{

    Random rand = new Random();
    private final Lock lock = new ReentrantLock();
    private int value = rand.nextInt(100);
    private int tentativas = 0;
    private boolean victory = false;
    private long tempoInicio = System.currentTimeMillis();


    public String adivinha(int n){
        lock.lock();
        try{
            tentativas++;
            if (System.currentTimeMillis() - tempoInicio > 60000) {
                return "TEMPO";
            }
            if(victory){
                return "PERDEU";
            }
            if(tentativas == 100){
                return "PERDEU";
            }
            if(tentativas > 100){
                return "TENTATIVAS";
            }
            if(n > value){
                return "MENOR";
            }
            if(n < value) {
                return "MAIOR";
            }
            if(n == value){
                victory = true;
                return "GANHOU";
            }
            return "ERRO";
        }
        finally{
            lock.unlock();
        }
    }
}

public class InitJogo implements Jogo{

    private int jogadores = 0;
    private final Lock lock = new ReentrantLock();
    private final Condition cond = lock.newCondition();


    public Partida participa(){
        lock.lock();
        try{
            jogadores++;
            while(jogadores < 4){
                cond.await();
            }
            cond.signalAll();
            return new Adivinha();
        }
        catch(InterruptedException e){
            e.printStackTrace();
            return null;
        }
        finally{
            lock.unlock();
        }
    }
}

public class Main {
    public static void main(String[] args) {
        //! falta a main e o resto não sei se está correto
    }
}


