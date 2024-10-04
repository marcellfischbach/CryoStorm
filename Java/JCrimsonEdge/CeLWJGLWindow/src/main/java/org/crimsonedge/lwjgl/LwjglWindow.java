package org.crimsonedge.lwjgl;

import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.window.IWindow;

public class LwjglWindow extends CoreObject implements IWindow {

    private String title;

    private final LwjglWindowCanvas canvas;

    private final LwjglMouse mouse;

    private final LwjglKeyboard keyboard;

    private static native long nCreate();
    private static native void nSetJObject (long ref, csObject obj);

    private static native LwjglMouse nGetMouse (long ref);
    private static native LwjglKeyboard nGetKeyboard(long ref);

    public LwjglWindow(LwjglWindowCanvas canvas) {
        super (nCreate());
        nSetJObject(getRef(), this);
        this.mouse = nGetMouse(getRef());
        this.keyboard = nGetKeyboard(getRef());
        this.canvas = canvas;
        this.mouse.setCanvas(canvas);
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
        return this.canvas.getRenderWidth();
    }

    @Override
    public int getHeight() {
        return this.canvas.getRenderHeight();
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

    public LwjglMouse getMouse() {
        return mouse;
    }

    public LwjglKeyboard getKeyboard() {
        return keyboard;
    }
}
