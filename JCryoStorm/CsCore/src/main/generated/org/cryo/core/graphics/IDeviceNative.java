package org.cryo.core.graphics;


/**
 * cs::iDevice
 */
public abstract class IDeviceNative {
    
    public static final String CS_CLASS_NAME = "cs::iDevice";
    
    private IDeviceNative() {
    }



    /**
     * void cs::iDevice::SetViewport(int16_t x, int16_t y, uint16_t width, uint16_t height)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param x x (int16_t)
     * @param y y (int16_t)
     * @param width width (uint16_t)
     * @param height height (uint16_t)
     */
    public static native void nSetViewport(long ref, short x, short y, int width, int height);

    /**
     * void cs::iDevice::SetColorWrite(bool redMask, bool greenMask, bool blueMask, bool alphaMask)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param redMask redMask (bool)
     * @param greenMask greenMask (bool)
     * @param blueMask blueMask (bool)
     * @param alphaMask alphaMask (bool)
     */
    public static native void nSetColorWrite(long ref, boolean redMask, boolean greenMask, boolean blueMask, boolean alphaMask);

    //##END-csMOC # Don't remove
}


