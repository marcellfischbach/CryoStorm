package org.crimsonedge.launcher;

import java.util.HashMap;
import java.util.Map;

public class Bootstrapper {

    public static void bootstrap() {
        Map<String, String> newEnv = new HashMap<>();

        System.loadLibrary("ceCore");
        System.loadLibrary("ceJavaBinding");
        nbootstrap();

    }

    private static native void nbootstrap();


}
