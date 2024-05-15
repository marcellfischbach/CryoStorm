package org.crimsonedge.launcher;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.window.IWindow;

@CeClass("ce::java::LwjglWindow")
public class LwjglWindow extends CoreObject implements IWindow {

    private String title;

    private final LwjglWindowCanvas canvas;

    public LwjglWindow(LwjglWindowCanvas canvas) {
        this.canvas = canvas;
    }

    @Override
    public void setTitle(String title) {
        this.title = title;
    }

    @Override
    public String getTitle() {
        return this.title;
    }

    @Override
    public void setPosition(int x, int y) {
        //
    }

    @Override
    public int getPositionX() {
        return 0;
    }

    @Override
    public int getPositionY() {
        return 0;
    }

    @Override
    public void setResolution(int width, int height) {

    }

    @Override
    public int getWidth() {
        return this.canvas.getWidth();
    }

    @Override
    public int getHeight() {
        return this.getHeight();
    }

    @Override
    public void show() {
        //
    }

    @Override
    public void hide() {
        //
    }

    @Override
    public void present() {
        //
    }

    @Override
    public void processUpdates() {
        //
    }
}
