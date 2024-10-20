package org.cryo.core.graphics;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass("cs::iDevice")
public class IDevice extends CsObject {

    public IDevice(long ref) {
        super(ref);
    }

    //##BEGIN-csMOC # Don't remove

    private static native void nSetViewport(long ref /* this ptr */,
                                            short x /* x (int16_t) */,
                                            short y /* y (int16_t) */,
                                            int width /* width (uint16_t) */,
                                            int height /* height (uint16_t) */
                                           );

    private static native void nSetColorWrite(long ref /* this ptr */,
                                              boolean redMask /* redMask (bool) */,
                                              boolean greenMask /* greenMask (bool) */,
                                              boolean blueMask /* blueMask (bool) */,
                                              boolean alphaMask /* alphaMask (bool) */
                                             );

    //##END-csMOC # Don't remove
}



