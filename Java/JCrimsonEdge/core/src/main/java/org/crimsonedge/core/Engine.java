package org.crimsonedge.core;

public class Engine {

    private static native boolean nInitialize (String[] args, IGame game);
    private static native void nProcessFrame ();


    public boolean initialize (String[] args, IGame game) {
        return nInitialize(args, game);
    }

    public void processFrame () {
        nProcessFrame ();
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

