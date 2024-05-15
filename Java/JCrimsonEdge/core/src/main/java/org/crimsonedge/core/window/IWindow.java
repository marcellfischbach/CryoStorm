package org.crimsonedge.core.window;

public interface IWindow {

    void setTitle (String title);
    String getTitle ();

    void setPosition (int x, int y);
    int getPositionX ();
    int getPositionY ();

    void setResolution (int width, int height);
    int getWidth();
    int getHeight();

    void show ();
    void hide ();

    void present ();
    void processUpdates();
}
