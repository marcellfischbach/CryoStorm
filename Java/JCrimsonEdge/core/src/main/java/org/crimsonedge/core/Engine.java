package org.crimsonedge.core;

import org.crimsonedge.core.entity.World;
import org.crimsonedge.core.graphics.IDevice;

public class Engine {

    private static native boolean nInitialize (String[] args, IGame game);
    private static native void nProcessFrame ();
    private static native World nGetWorld();
    private static native IDevice nGetDevice();


    public boolean initialize (String[] args, IGame game) {
        return nInitialize(args, game);
    }

    public void processFrame () {
        nProcessFrame ();
    }

    public World getWorld () {
        return nGetWorld();
    }

    public IDevice getDevice () {
        return nGetDevice();
    }

    private Engine() {
    }

    private static Engine instance = null;

    public static Engine instance() {
        if (instance == null) {
            instance = new Engine();
        }
        return instance;
    }

}

