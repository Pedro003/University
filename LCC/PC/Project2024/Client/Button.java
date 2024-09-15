package Client;

import processing.core.PApplet;

public class Button extends PApplet{
    PApplet parent;
    String label;
    float x, y, w, h;

    public Button(PApplet parent, String label, float x, float y, float w, float h) {
        this.parent = parent;
        this.label = label;
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
    }

    public void display() {
        parent.fill(200);
        parent.rect(x, y, w, h);
        parent.fill(0);
        parent.textAlign(PApplet.CENTER, PApplet.CENTER);
        parent.text(label, x + w / 2, y + h / 2);
    }

    public boolean isClicked() {
        return parent.mousePressed && parent.mouseX > x && parent.mouseX < x + w && parent.mouseY > y && parent.mouseY < y + h;
    }
}