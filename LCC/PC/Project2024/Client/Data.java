package Client;

import java.util.ArrayList;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import processing.core.PApplet;

enum Response {
    OK, 
    ERROR,
    SWITCH,
    NOTHING
}


public class Data {
    public Lock lock = new ReentrantLock();
    public Condition waitListener = lock.newCondition();
    public Condition waitScreen = lock.newCondition();
    
    public StateOption option; // Option selected by the user in the screen
    public StateScreen screen; // State of the screen

    public Response response = Response.NOTHING;

    public ArrayList<TPUser> leaderboard = new ArrayList<TPUser>();
    public ArrayList<Planet> planets = new ArrayList<>();
    public ArrayList<Player> players = new ArrayList<>();
    public PApplet Papplet;
    public String username;
    public String password;
}
