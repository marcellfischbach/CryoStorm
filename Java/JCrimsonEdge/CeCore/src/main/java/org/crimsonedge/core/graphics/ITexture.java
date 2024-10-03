package org.crimsonedge.core.graphics;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cryo::iTexture")
public class ITexture extends CoreObject  {

    public ITexture(long ref) {
        super(ref);
    }

    private static native int nGetTextureType (long ref);

    private static native int nGetPixelFormat (long ref);

    public ETextureType getTextureType () {
        int t = nGetTextureType(getRef());
        return ETextureType.values()[t];
    }

    public EPixelFormat getPixelFormat () {
        int t = nGetPixelFormat(getRef());
        return EPixelFormat.values()[t];
    }
}
