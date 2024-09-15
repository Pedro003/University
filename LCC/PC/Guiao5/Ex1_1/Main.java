import java.util.*;
import java.util.concurrent.locks.*;


class Warehouse {
    
    private final Lock lock = new ReentrantLock();
    private final Condition condition = lock.newCondition();
    private Map<String, Product> map =  new HashMap<String, Product>();

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
                while (get(s).quantity == 0) {
                    condition.await();
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
    public static void main(String[] args) {
        Warehouse warehouse = new Warehouse();

        // Supplier threads
        new Thread(() -> {
            try {
                for (int i = 0; i < 10; i++) {
                    warehouse.supply("item1", 1);
                    System.out.println("Supplied 1 of item1");
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();

        new Thread(() -> {
            try {
                for (int i = 0; i < 10; i++) {
                    warehouse.supply("item2", 1);
                    System.out.println("Supplied 1 of item2");
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();

        // Consumer thread
        new Thread(() -> {
            try {
                for (int i = 0; i < 10; i++) {
                    warehouse.consume(new HashSet<>(Arrays.asList("item3", "item2")));
                    System.out.println("Consumed set of items");
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
    }
}