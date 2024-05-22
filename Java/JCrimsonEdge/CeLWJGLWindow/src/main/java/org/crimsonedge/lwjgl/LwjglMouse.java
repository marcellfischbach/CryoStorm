package org.crimsonedge.lwjgl;

import org.crimsonedge.core.input.ECursorMode;
import org.crimsonedge.core.input.EMouseButton;
import org.crimsonedge.core.input.IMouse;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

public class LwjglMouse implements IMouse {

    private final long ref;

    private int posX;

    private int posY;

    private int lastX;
    private int lastY;

    private boolean visible = true;
    private ECursorMode cursorMode = ECursorMode.FREE;
    private Robot robot;

    public static final class Event {
        int button;
        boolean down;
    }

    private Event[] events = new Event[1000];
    private int numEvents = 0;

    private boolean[] buttons = new boolean[EMouseButton.values().length];
    private boolean[] prevButtons = new boolean[EMouseButton.values().length];

    private LwjglWindowCanvas canvas;
    private Cursor blankCursor;

    public LwjglMouse(long ref) {
        this.ref = ref;
        PointerInfo inf = MouseInfo.getPointerInfo();
        Point p = inf.getLocation();
        posX = lastX = p.x;
        posY = lastY = p.y;
        for (int i = 0; i < events.length; i++) {
            events[i] = new Event();
        }
        try {
            robot = new Robot();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        BufferedImage cursorImg = new BufferedImage(16, 16, BufferedImage.TYPE_INT_ARGB);

        this.blankCursor = Toolkit.getDefaultToolkit().createCustomCursor(cursorImg, new Point(0, 0), "blank cursor");
    }

    public LwjglWindowCanvas getCanvas() {
        return canvas;
    }

    public void setCanvas(LwjglWindowCanvas canvas) {
        this.canvas = canvas;
    }

    @Override
    public void setVisible(boolean visible) {
        this.visible = visible;
        if (visible) {
            this.canvas.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        }
        else {
            this.canvas.setCursor(this.blankCursor);
        }
    }

    @Override
    public boolean isVisible() {
        return this.visible;
    }

    public void setCursorMode(int mode) {
        setCursorMode(ECursorMode.values()[mode]);
    }

    @Override
    public void setCursorMode(ECursorMode mode) {
        if (this.cursorMode != mode) {
            this.cursorMode = mode;
            if (this.cursorMode == ECursorMode.FIXED) {
                Point center = new Point(this.canvas.getWidth() / 2, this.canvas.getHeight() / 2);
                SwingUtilities.convertPointToScreen(center, this.canvas);

                this.lastX = center.x;
                this.lastY = center.y;
                this.robot.mouseMove(lastX, lastY);
                this.posX = this.lastX;
                this.posY = this.lastY;
            }
        }
    }

    @Override
    public ECursorMode getCursorMode() {
        return ECursorMode.FREE;
    }

    @Override
    public int getX() {
        return this.posX;
    }

    @Override
    public int getY() {
        return this.posY;
    }

    @Override
    public int getDeltaX() {
        return this.posX - lastX;
    }

    @Override
    public int getDeltaY() {
        return this.posY - lastY;
    }

    @Override
    public int getWheel() {
        return 0;
    }

    @Override
    public int getWheelHorizontal() {
        return 0;
    }

    public boolean isButtonDown(int mouseButton) {
        return buttons[mouseButton];
    }

    public boolean isButtonUp(int mouseButton) {
        return !buttons[mouseButton];
    }

    public boolean isButtonPressed(int mouseButton) {
        return buttons[mouseButton] && !prevButtons[mouseButton];
    }

    public boolean isButtonReleased(int mouseButton) {
        return !buttons[mouseButton] && prevButtons[mouseButton];
    }

    @Override
    public boolean isButtonDown(EMouseButton mouseButton) {
        return isButtonDown(mouseButton.ordinal());
    }

    @Override
    public boolean isButtonUp(EMouseButton mouseButton) {
        return isButtonUp(mouseButton.ordinal());
    }

    @Override
    public boolean isButtonPressed(EMouseButton mouseButton) {
        return isButtonPressed(mouseButton.ordinal());
    }

    @Override
    public boolean isButtonReleased(EMouseButton mouseButton) {
        return isButtonReleased(mouseButton.ordinal());
    }

    public void pushEvent(int button, boolean down) {
        Event event = events[numEvents++];
        event.button = button;
        event.down = down;
    }

    public void update() {
        updateButtons();
        updatePosition();
    }

    private void updateButtons() {
        System.arraycopy(this.buttons, 0, this.prevButtons, 0, this.buttons.length);
        for (int i = 0; i < this.numEvents; i++) {
            Event evt = events[i];
            this.buttons[evt.button] = evt.down;
        }
        this.numEvents = 0;
    }

    private void updatePosition() {
        PointerInfo inf = MouseInfo.getPointerInfo();
        Point p = inf.getLocation();

        if (this.cursorMode == ECursorMode.FIXED) {
            if (this.robot != null) {
                posX = p.x;
                posY = p.y;

                Point center = new Point(this.canvas.getWidth() / 2, this.canvas.getHeight() / 2);
                SwingUtilities.convertPointToScreen(center, this.canvas);

                this.lastX = center.x;
                this.lastY = center.y;
                this.robot.mouseMove(lastX, lastY);
                return;
            }
        }
        else if (this.cursorMode == ECursorMode.CONFINED) {
            if (this.robot != null) {
                Point topLeft = new Point(0, 0);
                Point bottomRight = new Point(this.canvas.getWidth(), this.canvas.getHeight());
                SwingUtilities.convertPointToScreen(topLeft, this.canvas);
                SwingUtilities.convertPointToScreen(bottomRight, this.canvas);

                int newX = Math.min(Math.max(p.x, topLeft.x), bottomRight.x);
                int newY = Math.min(Math.max(p.y, topLeft.y), bottomRight.y);
                if (newX != p.x || newY != p.y) {
                    this.robot.mouseMove(newX, newY);
                }

                lastX = posX;
                lastY = posY;
                posX = newX;
                posY = newY;
                return;
            }
        }

        lastX = posX;
        lastY = posY;
        posX = p.x;
        posY = p.y;
    }


}
