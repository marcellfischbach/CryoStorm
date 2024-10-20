package org.cryo.core.resource;

public class VFSConfigReader {

    private static native void nRead (String rootPath, String configFileName);
    public static void read (String rootPath, String configFileName) {
        nRead(rootPath, configFileName);
    }
}
