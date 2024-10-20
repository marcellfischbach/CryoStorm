package org.crimsonedge.core.input;

public interface IMouse {

    void setVisible(boolean visible);

    boolean isVisible();

    void setCursorMode(ECursorMode mode);

    ECursorMode getCursorMode();


    int getX();

    int getY();

    int getDeltaX();

    int getDeltaY();

    int getWheel();

    int getWheelHorizontal();

    boolean isButtonDown(EMouseButton mouseButton);

    boolean isButtonUp(EMouseButton mouseButton);

    boolean isButtonPressed(EMouseButton mouseButton);

    boolean isButtonReleased(EMouseButton mouseButton);
}
