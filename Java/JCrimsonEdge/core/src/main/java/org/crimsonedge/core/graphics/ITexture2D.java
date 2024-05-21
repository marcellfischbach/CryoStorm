package org.crimsonedge.core.graphics;

public class ITexture2D extends ITexture {

    public static final class Descriptor {
        public EPixelFormat format;
        public int width;
        public int height;
        public boolean mipMaps;
        public int multiSamples;
    }

    public ITexture2D(long ref) {
        super(ref);
    }

    private static native boolean nIsMultiSampling(long ref);
    private static native int nGetSamples(long ref);
    private static native int nGetWidth(long ref);
    private static native int nGetHeight(long ref);

    private static native void nData (long ref, int level, int pixelFormat, byte[] data);
    private static native void nDataExt (long ref, int level, int x, int y, int width, int height, int pixelFormat, byte[] data);

    public boolean isMultiSampling() {
        return nIsMultiSampling(getRef());
    }

    public int getSamples () {
        return nGetSamples(getRef());
    }

    public int getWidth () {
        return nGetWidth(getRef());
    }

    public int getHeight () {
        return nGetHeight(getRef());
    }

    public void data (int level, EPixelFormat pixelFormat, byte[] data) {
        nData(getRef(), level, pixelFormat.ordinal(), data);
    }

    public void data (int level, int x, int y, int width, int height, EPixelFormat pixelFormat, byte[] data) {
        nDataExt(getRef(), level, x, y, width, height, pixelFormat.ordinal(), data);
    }
}
