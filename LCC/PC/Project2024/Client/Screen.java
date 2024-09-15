package Client;

import processing.core.PApplet;
import processing.core.PVector;

enum StateScreen {
    // States for the screen
    LRScreen,
    OptionScreen,
    LeaderboardScreen,
    GameScreen
}

enum StateOption {
    // States for the connection
    LOGIN,
    REGISTER,
    LOGOUT,
    LEADERBOARD,
    EXIT,
    PLAY,
    GAME,
    MOVE,
    END
}

public class Screen extends PApplet implements Runnable {

    private Data data;

    private int width = 1250;
    private int height = 800;
    private StateOption stateOption = null;

    private StateScreen stateScreen;

    // Login and Register Screen
    private Button loginButton, registerButton, exitButton;
    private TextBox usernameBox, passwordBox;

    // Options Screen
    private Button playButton, logoutButton, leaderBoardButton;

    // Leaderboard Screen
    private Button backButton;

    // Game Screen
    private PVector solPos;

    // Screen Constructor
    public Screen(Data data) {
        this.data = data;
        data.screen = StateScreen.LRScreen;
    }

    public void settings() {
        size(width, height);
    }

    public void setup() {
        // Login and Register Screen
        loginButton = new Button(this, "Login", width / 2 - 150, height / 2 + 50, 100, 40);
        registerButton = new Button(this, "Register", width / 2 + 40, height / 2 + 50, 100, 40);
        exitButton = new Button(this, "Exit", 10, height - 50, 100, 40);

        usernameBox = new TextBox(this, width / 2 - 130, height / 2 - 100, 250, 40, false, false);
        passwordBox = new TextBox(this, width / 2 - 130, height / 2 - 30, 250, 40, true, false);

        // Options Screen
        playButton = new Button(this, "Play", width / 2 - 60, height / 2 + 20, 120, 40);
        leaderBoardButton = new Button(this, "Leaderboard", width / 2 - 60, height / 2 + 100, 120, 40);
        logoutButton = new Button(this, "Logout", width / 2 - 60, height / 2 + 180, 120, 40);

        // Leaderboard Screen
        backButton = new Button(this, "Back", 10, height - 50, 100, 40);

        // Game Screen
        solPos = new PVector(625, 400);
    }

    public void draw() {
        background(255);

        switch (data.screen) {
            case LRScreen:
                LRScreen();
                break;
            case OptionScreen:
                OptionScreen();
                break;
            case LeaderboardScreen:
                LeaderboardScreen();
                break;
            case GameScreen:
                GameScreen();
                handleConnectionState(StateScreen.GameScreen, StateScreen.OptionScreen);
                break;
        }
    }

    // ------------------
    // Screens
    // ------------------

    public void LRScreen() {
        background(255);
        textAlign(CENTER);
        textSize(50);
        fill(0);
        text("Login/Register", width / 2, height / 2 - 200);

        textAlign(LEFT);
        textSize(15);
        fill(0);
        text("Username:", usernameBox.x, usernameBox.y - 10);
        text("Password:", passwordBox.x, passwordBox.y - 10);

        usernameBox.display();
        passwordBox.display();

        loginButton.display();
        registerButton.display();
        exitButton.display();
    }

    public void OptionScreen() {
        background(255);
        textAlign(CENTER);
        textSize(50);
        fill(0);
        text("Options", width / 2, height / 2 - 200);

        textAlign(LEFT);
        textSize(20);
        fill(0);
        playButton.display();
        logoutButton.display();
        leaderBoardButton.display();
    }

    public void LeaderboardScreen() {
        background(255);

        background(255);
        textAlign(CENTER);
        textSize(50);
        fill(0);
        text("Leaderboard", width / 2, height / 2 - 300);

        int largura = 500;
        int altura = 50;
        int espaco = 10;
        int x = (width - largura) / 2;
        int y = 130;

        for (TPUser user : data.leaderboard) {
            fill(200);
            rect(x, y, largura, altura);
            fill(0);
            textSize(20);
            textAlign(LEFT, CENTER);
            text(user.toString(), x + 10, y + altura / 2);
            y += altura + espaco;
        }

        backButton.display();
    }

    public void GameScreen() {
        background(0);
        fill(255, 204, 0);
        ellipse(solPos.x, solPos.y, 50, 50);

        data.lock.lock();
        try {
            for (Planet p : data.planets) {
                p.display();
            }
            for (Player p : data.players) {
                p.display();
                System.out.println(p.toString());
            }
        } finally {
            data.lock.unlock();
        }
    }

    // ------------------
    // Funções Auxiliares
    // ------------------
    // Game Screen, Option Screen
    public void handleConnectionState(StateScreen nextState, StateScreen errorState) {
        try {
            data.lock.lock();

            data.option = stateOption;
            data.Papplet = this;

            data.waitListener.signal();
            while (data.response == Response.NOTHING) {
                data.waitScreen.await();
            }

            if (data.response == Response.ERROR) {
                data.screen = errorState;
            } else if (data.response == Response.OK) {
                data.screen = nextState;
            } else if (data.response == Response.SWITCH) {
                data.screen = errorState;
                stateScreen = data.screen;
                stateOption = data.option;
            }

            data.response = Response.NOTHING;

        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            data.lock.unlock();
        }
    }

    @Override
    public void mousePressed() {
        switch (data.screen) {
            case LRScreen:
                if (loginButton.isClicked()) {
                    data.username = usernameBox.getText();
                    data.password = passwordBox.getText();
                    stateOption = StateOption.LOGIN;
                    handleConnectionState(StateScreen.OptionScreen, StateScreen.LRScreen);
                }
                if (registerButton.isClicked()) {
                    data.username = usernameBox.getText();
                    data.password = passwordBox.getText();
                    stateOption = StateOption.REGISTER;
                    handleConnectionState(StateScreen.OptionScreen, StateScreen.LRScreen);
                }
                if (exitButton.isClicked()) {
                    stateOption = StateOption.EXIT;
                    exit();
                }
                if (usernameBox.isMouseOver()) {
                    passwordBox.setActive(false);
                    usernameBox.setActive(true);
                } else if (passwordBox.isMouseOver()) {
                    usernameBox.setActive(false);
                    passwordBox.setActive(true);
                }
                break;
            case OptionScreen:
                if (playButton.isClicked()) {
                    stateOption = StateOption.PLAY;
                    handleConnectionState(StateScreen.GameScreen, StateScreen.OptionScreen);
                    stateOption = data.option;
                }
                if (logoutButton.isClicked()) {
                    stateOption = StateOption.LOGOUT;
                    handleConnectionState(StateScreen.LRScreen, StateScreen.OptionScreen);
                }
                if (leaderBoardButton.isClicked()) {
                    stateOption = StateOption.LEADERBOARD;
                    handleConnectionState(StateScreen.LeaderboardScreen, StateScreen.OptionScreen);
                }
                break;
            case LeaderboardScreen:
                if (backButton.isClicked()) {
                    handleConnectionState(StateScreen.OptionScreen, StateScreen.LeaderboardScreen);
                }
                break;
        }
    }

    public void keyTyped() {
        if (usernameBox.getActive() != false) {
            usernameBox.keyTyped();
        }
        if (passwordBox.getActive() != false) {
            passwordBox.keyTyped();
        }
    }

    public void keyPressed() {
        if (stateScreen == StateScreen.GameScreen) {
            if (keyCode == UP) {
                handleConnectionState(StateScreen.GameScreen, StateScreen.GameScreen);
                stateOption = StateOption.GAME;
            }
            if (keyCode == LEFT) {
                handleConnectionState(StateScreen.GameScreen, StateScreen.GameScreen);
                stateOption = StateOption.GAME;
            }
            if (keyCode == RIGHT) {
                handleConnectionState(StateScreen.GameScreen, StateScreen.GameScreen);
                stateOption = StateOption.GAME;
            }
        }
    }

    @Override
    public void run() {
        String[] processingArgs = { "Screen" };
        Screen screen = new Screen(data);
        PApplet.runSketch(processingArgs, screen);
    }
}
