package Client;

import processing.core.PApplet;
import processing.core.PVector;

public class Player extends PApplet {
    private PVector pos;
    private float radius = 10;
    private PApplet parent;

    Player(PApplet parent, float x, float y) {
        this.pos = new PVector(x, y);
        this.parent = parent;
    }

    void display() {
        parent.fill(255, 0, 0);
        parent.ellipse(pos.x, pos.y, radius * 2, radius * 2);
        parent.line(0, 0, radius, 0);
    }

    public String toString() {
        return String.format("Player at (%f, %f)", pos.x, pos.y);
    }
}
