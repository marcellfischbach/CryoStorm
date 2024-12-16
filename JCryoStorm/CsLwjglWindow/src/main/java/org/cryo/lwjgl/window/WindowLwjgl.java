package org.cryo.lwjgl.window;

import org.cryo.core.window.WindowJava;

public class WindowLwjgl extends WindowJava {

    private final AwtGlCanvas canvas;

    public WindowLwjgl(AwtGlCanvas canvas) {
        super();
        this.canvas = canvas;
    }

    @Override
    public void setTitle(String title) {
        //
    }

    @Override
    public String getTitle() {
        return "";
    }

    @Override
    public void setPosition(short x, short y) {
        //
    }

    @Override
    public short getPositionX() {
        return 0;
    }

    @Override
    public short getPositionY() {
        return 0;
    }

    @Override
    public void setResolution(int width, int height) {
        //
    }

    @Override
    public int getWidth() {
        return (short) this.canvas.getWidth();
    }

    @Override
    public int getHeight() {
        return (short) this.canvas.getHeight();
    }

    @Override
    public void show() {

    }

    @Override
    public void hide() {

    }

    @Override
    public void present() {
        canvas.swapBuffers();
    }

    @Override
    public void processUpdates() {
        // hmm... don't know yet
    }
}