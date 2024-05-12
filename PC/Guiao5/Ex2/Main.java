import java.util.*;
import java.util.concurrent.locks.*;


class Warehouse {
    
    private final Lock lock = new ReentrantLock();
    private final Condition condition = lock.newCondition();
    private HashMap<String, Product> map =  new HashMap<String, Product>();
    private HashMap<String, Integer> stock = new HashMap<String, Integer>();

    private class Product {
        int quantity = 0; 
    }

    private Product get(String item) {
        Product p = map.get(item);
        if (p != null) return p;
        p = new Product();
        map.put(item, p);
        return p;
    }

    public void supply(String item, int quantity) throws InterruptedException {
        lock.lock();
        try{
            Product p = get(item);
            p.quantity += quantity;
            condition.signalAll();
        }
        finally{
            lock.unlock();
        }
    }

    // Errado se faltar algum produto...
    public void consume(Set<String> items) throws InterruptedException {
        lock.lock();
        try{
            for (String s : items){
                if(stock.get(s) == null && get(s).quantity == 0) stock.put(s, 0);
                if (get(s).quantity == 0){
                    int stock_increment = stock.get(s) + 1;
                    stock.put(s, stock_increment);
                    while (get(s).quantity < stock.get(s)) {
                        condition.await();
                    }
                    stock_increment--;
                    stock.put(s, stock_increment);
                }
                get(s).quantity--;
            }
        }
        finally{
            lock.unlock();
        }
    }
}


public class Main {
    public static void main(String[] args) throws InterruptedException {
        
        int N = 10;
        Warehouse warehouse = new Warehouse();
        Random random = new Random();
        String[] array = {"Deagle", "AWP", "M4-A1S", "AK", "USP", "Glock", "P90", "UMP", "MP9", "MP7"};

        // Supplier threads
        new Thread(() -> {
            try {
                for (int i = 0; i < 100; i++) {
                    int inds = random.nextInt(N);
                    warehouse.supply(array[inds], 1);
                    System.out.println("Supplied 1 of " + array[inds]);
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();

        Runnable consume = () -> {
            
            try {
                int indc1 = random.nextInt(N-1);
                int indc2 = random.nextInt(N-1);
                System.out.println("Trying to get: " + array[indc1] + " and " + array[indc2]);
                warehouse.consume(new HashSet<>(Arrays.asList(array[indc1] , array[indc2])));
                System.out.println("Aquired: " + array[indc1] + " and " + array[indc2]);
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            
            }
        };

        for (int i = 0; i < N; i++) {
            new Thread(consume).start();
        }
    }
}