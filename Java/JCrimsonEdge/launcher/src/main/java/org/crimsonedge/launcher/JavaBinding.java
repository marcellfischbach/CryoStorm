package org.crimsonedge.launcher;

import java.util.HashMap;
import java.util.Map;

public class JavaBinding {

    public static void loadLibraries() {
        Map<String, String> newEnv = new HashMap<>();

        System.loadLibrary("ceCore");
        System.loadLibrary("ceJavaBinding");

    }

    public static void setEnv () {
        nSetEnv();
    }

    private static native void nSetEnv();


}
