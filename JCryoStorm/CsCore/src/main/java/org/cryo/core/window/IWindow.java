package org.cryo.core.window;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass(IWindowNative.CS_CLASS_NAME)
public class IWindow extends CsObject {

    public IWindow() {
    }

    public IWindow(long ref) {
        super(ref);
    }

    public void setTitle(String title) {
        IWindowNative.nSetTitle(getRef(), title);
    }

    public String getTitle() {
        return IWindowNative.nGetTitle(getRef());
    }

    public void setPosition (short x, short y) {
        IWindowNative.nSetPosition(getRef(), x, y);
    }

    public short getPositionX () {
        return IWindowNative.nGetPositionX(getRef());
    }

    public short getPositionY () {
        return IWindowNative.nGetPositionY(getRef());
    }

    public void setResolution (int width, int height) {
        IWindowNative.nSetResolution(getRef(), width, height);
    }

    public int getWidth () {
        return IWindowNative.nGetWidth(getRef());
    }

    public int getHeight () {
        return IWindowNative.nGetHeight(getRef());
    }

    public void show () {
        IWindowNative.nShow(getRef());
    }

    public void hide () {
        IWindowNative.nHide(getRef());
    }

    public void present () {
        IWindowNative.nPresent(getRef());
    }

    public void processUpdates () {
        IWindowNative.nProcessUpdates(getRef());
    }


}
