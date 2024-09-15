package Client;

import processing.core.PApplet;

public class TextBox {
    PApplet parent;
    float x, y, w, h;
    String text = "";
    boolean cursorVisible = true;
    long lastBlinkTime = 0;
    int cursorPosition = 0;
    boolean isPassword;
    boolean isActive = false;

    public TextBox(PApplet parent, float x, float y, float w, float h, boolean isPassword, boolean isActive) {
        this.parent = parent;
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
        this.isPassword = isPassword;
        this.isActive = isActive;
    }

    public void setActive(boolean isActive) {
        this.isActive = isActive;
    }

    public String getText() {
        return this.text;
    }

    public boolean getActive() {
        return this.isActive;
    }

    public void display() {
        parent.stroke(0);
        parent.strokeWeight(1);
        parent.fill(255);
        parent.rect(x, y, w, h);

        parent.fill(0);
        parent.textAlign(PApplet.LEFT, PApplet.CENTER);
        parent.textSize(20);
        String displayText = isPassword ? "*".repeat(text.length()) : text;
        parent.text(displayText, x + 5, y + h / 2);

        if (this.isActive && cursorVisible) {
            float cursorX = x + 5 + parent.textWidth(displayText.substring(0, cursorPosition));
            parent.stroke(0);
            parent.line(cursorX, y + 5, cursorX, y + h - 5);
        }

        if (parent.millis() - lastBlinkTime >= 500) {
            cursorVisible = !cursorVisible;
            lastBlinkTime = parent.millis();
        }
    }

    public void keyTyped() {
        if (parent.key != PApplet.CODED) {
            if (parent.key == PApplet.BACKSPACE) {
                if (text.length() > 0 && cursorPosition > 0) {
                    text = text.substring(0, cursorPosition - 1) + text.substring(cursorPosition);
                    cursorPosition--;
                }
            } else {
                String newText = text.substring(0, cursorPosition) + parent.key + text.substring(cursorPosition);
                float newTextWidth = parent.textWidth(newText);

                if (newTextWidth < w - 10) {
                    text = newText;
                    cursorPosition++;
                }
            }
        }
    }

    public boolean isMouseOver() {
        return parent.mouseX > x && parent.mouseX < x + w && parent.mouseY > y && parent.mouseY < y + h;
    }
}
