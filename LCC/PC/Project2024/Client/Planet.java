package Client;

import processing.core.PApplet;
import processing.core.PVector;

public class Planet extends PApplet {
    PApplet parent;
    private PVector pos = new PVector();
    private float radius = 20;

    public Planet(PApplet parent, float posX, float posY) {
        this.parent = parent;
        this.pos.x = posX;
        this.pos.y = posY;
    }

    public void display() {
        parent.fill(255);
        parent.ellipse(pos.x, pos.y, radius * 2, radius * 2);
    }

    public String toString() {
        return String.format("Planet at (%f, %f)", pos.x, pos.y);
    }
}