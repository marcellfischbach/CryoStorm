package org.crimsonedge.core;

import org.crimsonedge.core.entity.csWorld;
import org.crimsonedge.core.graphics.IDevice;

public class csEngine {

    private static native void nSetEnv ();
    private static native boolean nInitialize (String[] args, long externalModuleRef, IGame game);
    private static native void nProcessFrame ();
    private static native csWorld nGetWorld();
    private static native IDevice nGetDevice();


    public boolean initialize (String[] args, IModule externalModule, IGame game) {
        return nInitialize(args, externalModule != null ? externalModule.getRef() : 0, game);
    }

    public void processFrame () {
        nProcessFrame ();
    }

    public csWorld getWorld () {
        return nGetWorld();
    }

    public IDevice getDevice () {
        return nGetDevice();
    }

    private csEngine() {
        System.loadLibrary("ceCore");
    }


    public void setEnv () {
        nSetEnv();
    }

    private static csEngine instance = null;

    public static csEngine instance() {
        if (instance == null) {
            instance = new csEngine();
        }
        return instance;
    }

}

