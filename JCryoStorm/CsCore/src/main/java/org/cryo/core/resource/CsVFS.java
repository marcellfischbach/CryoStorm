package org.cryo.core.resource;

public class CsVFS
{



    private static native void nSetRootPath(String path);

    private static native String nGetRootPath();

    public void setRootPath (String rootPath) {
        nSetRootPath(rootPath);
    }

    public String getRootPath () {
        return nGetRootPath();
    }


    private CsVFS() {
    }

    private static CsVFS instance = null;

    public static CsVFS instance () {
        if (instance == null) {
            instance = new CsVFS();
        }
        return instance;
    }
}
