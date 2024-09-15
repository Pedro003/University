package Client;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;

import Client.Exceptions.InvalidUsername;
import Client.Exceptions.UserAlreadyExists;
import Client.Exceptions.InvalidPassword;


public class Connection {
    private Socket socket;
    // BufferedReader reads text from the server
    private BufferedReader in;
    // PrintWriter prints formatted representations to the server
    private PrintWriter out;


    public Connection(String host, int port) throws IOException {
        this.socket = new Socket(host, port);
        this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.out = new PrintWriter(socket.getOutputStream());
    }

    // Sends a message to the server
    public void send(String message) throws IOException {
        out.println(message);
        out.flush();
    }

    // Receives a message from the server
    public String receive() throws IOException {
        return in.readLine();
    }
    
    // Registers a user
    public void register(String username, String password) throws IOException, UserAlreadyExists, InvalidPassword {
        String message = String.format("register %s %s", username, password);
        send(message);

        switch (this.receive()) {
            case "register_ok":
                break;
            case "user_already_exists":
                throw new UserAlreadyExists("User already exists");
        }
    }

    // Logs in a user
    public void login(String username, String password) throws IOException, InvalidUsername, InvalidPassword {
        String message = String.format("login %s %s", username, password);
        send(message);

        switch (this.receive()) {
            case "login_ok":
                break;
            case "invalid_account":
                throw new InvalidUsername("Invalid username");
            case "invalid_password":
                throw new InvalidPassword("Invalid password");
        }
    }

    // Closes the connection ---> ver melhor
    public void close() throws IOException {
        send("Exit");
        if(this.receive().equals("Exiting")) {
            System.out.println("Connection closed");
            in.close();
            out.close();
            socket.close();
        }
    }

    // Logs out a user
    public void logout() throws IOException{
        send("Logout");
    }

    // Returns the leaderboard
    public ArrayList<TPUser> topPlayers() throws IOException {
        send("Leaderboard");
        ArrayList<TPUser> leaderboard = new ArrayList<TPUser>();
        String info = receive();
        String[] data2 = info.split(",");
        for (String d : data2) {
            String[] partes = d.split("\\|");
            String username = partes[0];
            int nivel = Integer.parseInt(partes[1]);
            int vitorias = Integer.parseInt(partes[2]);
            int derrotas = Integer.parseInt(partes[3]);
    
            TPUser tpUser = new TPUser(username, nivel, vitorias, derrotas);
            leaderboard.add(tpUser);
        }
        return leaderboard;
    }

    // Joins the lobby
    public void joinLobby() throws IOException {
        send("Play");
    }

    // Leaves the lobby
    public void leaveLobby() throws IOException {
        send("Leave");
    }

}
