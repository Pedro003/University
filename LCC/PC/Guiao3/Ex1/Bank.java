import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.Map;
import java.util.HashMap;

public class Bank {
    private static class Account {
        int balance;
        ReentrantReadWriteLock lock = new ReentrantReadWriteLock();

        Account(int balance) {
            this.balance = balance;
        }

        int balance() {
            lock.readLock().lock();
            try {
                return balance;
            } finally {
                lock.readLock().unlock();
            }
        }

        boolean deposit(int value) {
            lock.writeLock().lock();
            try {
                balance += value;
                return true;
            } finally {
                lock.writeLock().unlock();
            }
        }

        boolean withdraw(int value) {
            lock.writeLock().lock();
            try {
                if (value > balance) {
                    return false;
                }
                balance -= value;
                return true;
            } finally {
                lock.writeLock().unlock();
            }
        }
    }

    private Map<Integer, Account> map = new HashMap<>();
    private int nextId = 0;

    public int createAccount(int balance) {
        Account acc = new Account(balance);
        int id = nextId++;
        map.put(id, acc);
        return id;
    }

    public int closeAccount(int id) {
        Account acc = map.remove(id);
        if (acc == null) {
            return -1;
        }
        acc.lock.writeLock().lock();
        try {
            return acc.balance();
        } finally {
            acc.lock.writeLock().unlock();
        }
    }

    public int balance(int id) {
        Account acc = map.get(id);
        if (acc == null) {
            return 0;
        }
        return acc.balance();
    }

    public boolean deposit(int id, int value) {
        Account acc = map.get(id);
        if (acc == null) {
            return false;
        }
        return acc.deposit(value);
    }

    public boolean withdraw(int id, int value) {
        Account acc = map.get(id);
        if (acc == null) {
            return false;
        }
        return acc.withdraw(value);
    }

    public boolean transfer(int from, int to, int value) {
        Account accFrom = map.get(from);
        Account accTo = map.get(to);
        if (accFrom == null || accTo == null) {
            return false;
        }
        accFrom.lock.writeLock().lock();
        try {
            if (!accFrom.withdraw(value)) {
                return false;
            }
            accTo.lock.writeLock().lock();
            try {
                return accTo.deposit(value);
            } finally {
                accTo.lock.writeLock().unlock();
            }
        } finally {
            accFrom.lock.writeLock().unlock();
        }
    }

    public int totalBalance() {
        int total = 0;
        for (int id : map.keySet()) {
            Account acc = map.get(id);
            if (acc != null) {
                acc.lock.readLock().lock();
                try {
                    total += acc.balance();
                } finally {
                    acc.lock.readLock().unlock();
                }
            }
        }
        return total;
    }
}
