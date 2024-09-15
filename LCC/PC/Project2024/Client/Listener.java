package Client;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import Client.Exceptions.InvalidUsername;
import Client.Exceptions.UserAlreadyExists;
import processing.core.PApplet;
import Client.Exceptions.InvalidPassword;

public class Listener implements Runnable {

    private Connection connection;
    private Data data;

    public Listener(Connection connection, Data data) {
        this.connection = connection;
        this.data = data;
    }

    public void run() {
        while (true) {
            try {
                data.lock.lock();
                data.waitListener.await();

                switch (data.option) {
                    case LOGIN:
                        connection.login(data.username, data.password);
                        data.response = Response.OK;
                        break;
                    case REGISTER:
                        connection.register(data.username, data.password);
                        data.response = Response.OK;
                        break;
                    case LOGOUT:
                        connection.logout();
                        data.response = Response.OK;
                        break;
                    case LEADERBOARD:
                        data.leaderboard.clear();
                        data.leaderboard = connection.topPlayers();
                        data.response = Response.OK;
                        break;
                    case EXIT:
                        connection.close();
                        return;
                    case PLAY:
                        connection.joinLobby();
                        String serverLobby = connection.receive();
                        switch (serverLobby) {
                            case "Start_ok":
                                System.out.println("Game started");
                                data.option = StateOption.GAME;
                                break;
                        }
                        data.response = Response.OK;
                        break;
                    case GAME:

                        data.response = Response.OK;
                        String dataGame;
                        // connection.send("Update_planet");
                        String serverGame = connection.receive();
                        if (serverGame.equals("I_lost")) {
                            data.option = StateOption.END;
                            data.response = Response.SWITCH;
                        } else if (serverGame.equals("Won")) {
                            data.option = StateOption.END;
                            data.response = Response.SWITCH;
                        } else {
                            dataGame = connection.receive();
                            // System.out.println("Aqui" + dataGame);
                            dataGame = dataGame.substring(1, dataGame.length() - 1);

                            // Separar as coordenadas em pares usando "}, {" como delimitador
                            String[] pares = dataGame.split("\\}, \\{");

                            int tam = pares.length;
                            if (tam == 5) {
                                data.planets.clear();
                                // Ler cada par de coordenadas e criar objetos Ponto
                                for (String par : pares) {
                                    // Remover qualquer chaves remanescente no início ou fim do par
                                    par = par.replaceAll("[{}]", "");
                                    String[] valores = par.split(",");
                                    float x = Float.parseFloat(valores[0].trim());
                                    float y = Float.parseFloat(valores[1].trim());
                                    data.planets.add(new Planet(data.Papplet, x, y));
                                }

                            } else {

                                data.players.clear();

                                for (String par : pares) {
                                    // Remover qualquer chaves remanescente no início ou fim do par
                                    par = par.replaceAll("[{}]", "");
                                    String[] valores = par.split(",");
                                    float x = Float.parseFloat(valores[0].trim());
                                    System.out.println("X: " + x);
                                    float y = Float.parseFloat(valores[1].trim());
                                    System.out.println("Y: " + y);
                                    Player player = new Player(data.Papplet, x, y);
                                    System.out.println(player.toString());
                                    data.players.add(player);
                                }
                            }
                        }
                        break;
                    case END:
                        data.response = Response.OK;
                        break;
                }
                data.waitScreen.signal();
            } catch (InterruptedException | IOException e) {
                throw new RuntimeException(e);
            } catch (InvalidUsername | UserAlreadyExists | InvalidPassword e) {
                data.response = Response.ERROR;
                data.waitScreen.signal();
            } finally {
                data.lock.unlock();
            }
        }
    }
}
