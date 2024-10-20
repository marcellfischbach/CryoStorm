package org.crimsonedge.lwjgl;

import org.crimsonedge.core.IModule;

public class LwjglModule extends IModule {

    private static native long nCreateLwjglModule();

    public LwjglModule() {
        super(nCreateLwjglModule());
    }
}
