package org.crimsonedge.core;

import org.crimsonedge.core.entity.World;
import org.crimsonedge.core.graphics.IDevice;

public class Engine {

    private static native void nSetEnv ();
    private static native boolean nInitialize (String[] args, long externalModuleRef, IGame game);
    private static native void nProcessFrame ();
    private static native World nGetWorld();
    private static native IDevice nGetDevice();


    public boolean initialize (String[] args, IModule externalModule, IGame game) {
        return nInitialize(args, externalModule != null ? externalModule.getRef() : 0, game);
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
        System.loadLibrary("ceCore");
    }


    public void setEnv () {
        nSetEnv();
    }

    private static Engine instance = null;

    public static Engine instance() {
        if (instance == null) {
            instance = new Engine();
        }
        return instance;
    }

}

