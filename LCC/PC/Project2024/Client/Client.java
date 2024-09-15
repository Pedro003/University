package Client;

public class Client{

    public static void main(String[] args) {
        try {
            Connection connection = new Connection("localhost", 1234);
            Data data = new Data();
            Screen screen = new Screen(data);

            new Thread(screen).start();

            new Thread(new Listener(connection, data)).start();


        } catch (Exception e) {
            e.printStackTrace();
            System.exit(0);
        }
    }

}