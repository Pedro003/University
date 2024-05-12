public class Bank {

    private static class Account {
        private int balance;
        Account(int balance) {
            this.balance = balance; 
        }
        int balance() {
            return balance;
        }
        boolean deposit(int value) {
            balance += value;
            return true;
        }
        boolean withdraw(int value) {
            if (value > balance)
                return false;
            balance -= value;
            return true;
        }
    }

    // Bank slots and vector of accounts
    private int slots;
    private Account[] av; 

    public Bank(int n) {
        slots=n;
        av=new Account[slots];
        for (int i=0; i<slots; i++) av[i]=new Account(0);
    }

    // Account balance
    public int balance(int id) {
        if (id < 0 || id >= slots)
            return 0;
        return av[id].balance();
    }

    // Deposit
    public boolean deposit(int id, int value) {
        if (id < 0 || id >= slots)
            return false;
        return av[id].deposit(value);
    }

    // Tranfer
    public synchronized boolean transfer(int from, int to, int value) {
        if (withdraw(from, value))
            return deposit(to, value);
        return false;
    }

    // Withdraw fails if no such account or insufficient balance
    public boolean withdraw(int id, int value) {
        if (id < 0 || id >= slots)
            return false;
        return av[id].withdraw(value);
    }

    // Total balance
    public synchronized int totalBalance() {
        int total=0;
        for (int i=0; i<slots; i++)
            total += av[i].balance();
        return total;
    }
}