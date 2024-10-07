package org.crimsonedge.core.graphics;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cs::iDevice")
public class IDevice extends CoreObject {

    public IDevice(long ref) {
        super(ref);
    }


    // ******************************************************************************
    //           Creators
    // ******************************************************************************

    private static native ITexture2D nCreateTexture2D(long ref,
                                                      int pixelFormat,
                                                      int width,
                                                      int height,
                                                      boolean mipMaps,
                                                      int multiSamples);

    public ITexture2D createTexture(ITexture2D.Descriptor descriptor) {
        return nCreateTexture2D(getRef(),
                                descriptor.format.ordinal(),
                                descriptor.width,
                                descriptor.height,
                                descriptor.mipMaps,
                                descriptor.multiSamples);
    }
}
