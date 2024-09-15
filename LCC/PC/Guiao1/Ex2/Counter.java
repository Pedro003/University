class Counter {
    private int count = 0;

    public synchronized void increment() { // Para a versão 2 é só tirar o synchronized
        count++;
    }

    public int getCount() {
        return count;
    }
}