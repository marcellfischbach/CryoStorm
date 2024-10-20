package org.cryo.core.graphics;


/**
 * cs::iLight
 */
public abstract class ILightNative {
    
    public static final String CS_CLASS_NAME = "cs::iLight";
    
    private ILightNative() {
    }



    /**
     * cs::eLightType cs::iLight::GetType()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native int nGetType(long ref);

    /**
     * void cs::iLight::SetChangeMode(cs::eLightChangeMode changeMode)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param changeMode changeMode (cs::eLightChangeMode)
     */
    public static native void nSetChangeMode(long ref, int changeMode);

    /**
     * cs::eLightChangeMode cs::iLight::GetChangeMode()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native int nGetChangeMode(long ref);

    /**
     * void cs::iLight::SetValue(cs::eLightChangeMode changeMode, cs::eLightType type)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param changeMode changeMode (cs::eLightChangeMode)
     * @param type type (cs::eLightType)
     */
    public static native void nSetValue(long ref, int changeMode, int type);

    /**
     * void cs::iLight::SetCastShadow(bool castShadow)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param castShadow castShadow (bool)
     */
    public static native void nSetCastShadow(long ref, boolean castShadow);

    /**
     * bool cs::iLight::IsCastShadow()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native boolean nIsCastShadow(long ref);

    /**
     * void cs::iLight::SetShadowMapBias(float bias)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param bias bias (float)
     */
    public static native void nSetShadowMapBias(long ref, float bias);

    /**
     * float cs::iLight::GetShadowMapBias()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native float nGetShadowMapBias(long ref);

    /**
     * void cs::iLight::SetColor(const cs::csColor4f& color)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param colorR color (const cs::csColor4f&)
     * @param colorG color (const cs::csColor4f&)
     * @param colorB color (const cs::csColor4f&)
     * @param colorA color (const cs::csColor4f&)
     */
    public static native void nSetColor(long ref, float colorR, float colorG, float colorB, float colorA);

    /**
     * const cs::csColor4f& cs::iLight::GetColor()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param outArg0 cs::csColor4f (output return value)
     */
    public static native void nGetColor(long ref, float[] outArg0);

    /**
     * void cs::iLight::SetIntensity(float intensity)
     *
     * @param ref Reference pointer to this object: Usually getRef()
     * @param intensity intensity (float)
     */
    public static native void nSetIntensity(long ref, float intensity);

    /**
     * float cs::iLight::GetIntensity()
     *
     * @param ref Reference pointer to this object: Usually getRef()
     */
    public static native float nGetIntensity(long ref);

    //##END-csMOC # Don't remove
}


