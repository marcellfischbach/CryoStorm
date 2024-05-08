package org.crimsonedge.launcher;

import org.crimsonedge.core.entity.Entity;

import java.lang.reflect.Field;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class Bootstrapper {

    public static void bootstrap() {
        Map<String, String> newEnv = new HashMap<>();

        System.loadLibrary("ceCore");
        System.loadLibrary("ceJavaBinding");
        nbootstrap();

        Entity.doSomething();
    }

    private static native void nbootstrap();


}
