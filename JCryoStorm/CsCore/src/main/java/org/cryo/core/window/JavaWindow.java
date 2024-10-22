package org.cryo.core.window;


import org.cryo.core.CsClass;
import org.cryo.core.ICsObject;

@CsClass(JavaWindowNative.CS_CLASS_NAME)
public class JavaWindow extends IWindow {

    public JavaWindow() {
    }

    public JavaWindow(long ref) {
        super(ref);
    }

    @Override
    public void setTitle(String title) {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public String getTitle() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public void setPosition(short x, short y) {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public short getPositionX() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public short getPositionY() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public void setResolution(int width, int height) {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public int getWidth() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public int getHeight() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public void show() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public void hide() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public void present() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }

    @Override
    public void processUpdates() {
        throw new UnsupportedOperationException("To be implemented in subclass");
    }
}
