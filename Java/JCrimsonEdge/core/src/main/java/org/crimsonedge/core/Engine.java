package org.crimsonedge.core;

import org.crimsonedge.core.entity.World;

public class Engine {

    private static native boolean nInitialize (String[] args, IGame game);
    private static native void nProcessFrame ();
    private static native World nGetWorld();


    public boolean initialize (String[] args, IGame game) {
        return nInitialize(args, game);
    }

    public void processFrame () {
        nProcessFrame ();
    }

    public World getWorld () {
        return nGetWorld();
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

