package org.cryo.core.entity;


/**
 * cs::csLightState
 */
public abstract class LightStateNative {
    
    public static final String CS_CLASS_NAME = "cs::csLightState";
    
    private LightStateNative() {
    }



    /**
     * void cs::csLightState::SetCastShadow(bool castShadow)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param castShadow castShadow (bool)
     */
    public static native void nSetCastShadow(long ref, boolean castShadow);

    /**
     * bool cs::csLightState::IsCastShadow()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native boolean nIsCastShadow(long ref);

    /**
     * void cs::csLightState::SetShadowMapBias(float bias)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param bias bias (float)
     */
    public static native void nSetShadowMapBias(long ref, float bias);

    /**
     * float cs::csLightState::GetShadowMapBias()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native float nGetShadowMapBias(long ref);

    /**
     * void cs::csLightState::SetColor(const cs::csColor4f& color)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param colorR color (const cs::csColor4f&)
     * @param colorG color (const cs::csColor4f&)
     * @param colorB color (const cs::csColor4f&)
     * @param colorA color (const cs::csColor4f&)
     */
    public static native void nSetColor(long ref, float colorR, float colorG, float colorB, float colorA);

    /**
     * const cs::csColor4f& cs::csLightState::GetColor()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param outArg0 cs::csColor4f (output return value)
     */
    public static native void nGetColor(long ref, float[] outArg0);

    /**
     * void cs::csLightState::SetRange(float range)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param range range (float)
     */
    public static native void nSetRange(long ref, float range);

    /**
     * float cs::csLightState::GetRange()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native float nGetRange(long ref);

    //##END-csMOC # Don't remove
}


