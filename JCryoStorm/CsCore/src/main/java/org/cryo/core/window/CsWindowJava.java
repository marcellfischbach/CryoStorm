package org.cryo.core.window;


import org.cryo.core.CsClass;

import static org.cryo.core.window.CsWindowJavaNative.*;

@CsClass(CS_CLASS_NAME)
public class CsWindowJava extends IWindow {

    public CsWindowJava() {
    }

    public CsWindowJava(long ref) {
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
