package org.cryo.core.resource;

public class VFS {



    private static native void nSetRootPath(String path);

    private static native String nGetRootPath();

    public void setRootPath (String rootPath) {
        nSetRootPath(rootPath);
    }

    public String getRootPath () {
        return nGetRootPath();
    }


    private VFS () {
    }

    private static VFS instance = null;

    public static VFS instance () {
        if (instance == null) {
            instance = new VFS();
        }
        return instance;
    }
}
