package org.crimsonedge.launcher;

import org.crimsonedge.core.input.ECursorMode;
import org.crimsonedge.core.input.EMouseButton;
import org.crimsonedge.core.input.IMouse;

public class LwjglMouse implements IMouse {

    @Override
    public void setVisible(boolean visible) {

    }

    @Override
    public boolean isVisible() {
        return false;
    }

    @Override
    public void setCursorMode(ECursorMode mode) {

    }

    @Override
    public ECursorMode getCursorMode() {
        return null;
    }

    @Override
    public int getX() {
        return 0;
    }

    @Override
    public int getY() {
        return 0;
    }

    @Override
    public int getDeltaX() {
        return 1;
    }

    @Override
    public int getDeltaY() {
        return 1;
    }

    @Override
    public int getWheel() {
        return 0;
    }

    @Override
    public int getWheelHorizontal() {
        return 0;
    }

    @Override
    public boolean isButtonDown(EMouseButton mouseButton) {
        return mouseButton == EMouseButton.LEFT;
    }

    @Override
    public boolean isButtonUp(EMouseButton mouseButton) {
        return false;
    }

    @Override
    public boolean isButtonPressed(EMouseButton mouseButton) {
        return false;
    }

    @Override
    public boolean isButtonReleased(EMouseButton mouseButton) {
        return false;
    }
}
