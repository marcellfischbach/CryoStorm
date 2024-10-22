package org.cryo.core;

import org.cryo.core.entity.World;
import org.cryo.core.graphics.IDevice;
import org.cryo.core.graphics.IFrameRenderer;
import org.cryo.core.window.IWindow;

@CsClass(ViewportNative.CS_CLASS_NAME)
public class Viewport extends CsObject {

    public Viewport() {
    }

    public Viewport(long ref) {
        super(ref);
    }

    public void setDevice(IDevice device) {
        ViewportNative.nSetDevice(getRef(), device != null ? device.getRef() : 0);
    }

    public IDevice getDevice() {
        Object device = ViewportNative.nGetDevice(getRef());
        return device instanceof IDevice ? (IDevice) device : null;
    }

    public void setWorld(World world) {
        ViewportNative.nSetWorld(getRef(), world != null ? world.getRef() : 0);
    }

    public World getWorld() {
        Object world = ViewportNative.nGetWorld(getRef());
        return world instanceof World ? (World) world : null;
    }

    public void setFrameRenderer(IFrameRenderer frameRenderer) {
        ViewportNative.nSetFrameRenderer(getRef(), frameRenderer != null ? frameRenderer.getRef() : 0);
    }

    public IFrameRenderer getFrameRenderer() {
        Object frameRenderer = ViewportNative.nGetFrameRenderer(getRef());
        return frameRenderer instanceof IFrameRenderer ? (IFrameRenderer) frameRenderer : null;
    }


    public void setWindow(IWindow window) {
        ViewportNative.nSetWindow(getRef(), window != null ? window.getRef() : 0);
    }

    public IWindow getWindow() {
        Object window = ViewportNative.nGetWindow(getRef());
        return window instanceof IWindow ? (IWindow) window : null;
    }

    public void processFrame () {
        ViewportNative.nProcessFrame(getRef());
    }

}
