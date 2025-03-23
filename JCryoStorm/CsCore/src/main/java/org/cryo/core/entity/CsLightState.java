package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.ELightType;
import org.cryo.core.math.CsColor4f;


import static org.cryo.core.entity.CsLightStateNative.*;

@CsClass(CS_CLASS_NAME)
public class CsLightState extends CsSpatialState
{

    public CsLightState() {
    }

    public CsLightState(long ref) {
        super(ref);
    }

    public void setType (ELightType lightType) {
        nSetType(getRef(), lightType.ordinal());
    }

    public ELightType getType () {
        return ELightType.values()[nGetType(getRef())];
    }

    public void setColor(CsColor4f color) {
        nSetColor(getRef(), color.r, color.g, color.b, color.a);
    }

    private static float[] colorBuf = new float[4];
    private final CsColor4f color = new CsColor4f();
    public CsColor4f getColor () {
        nGetColor(getRef(), colorBuf);
        color.set(colorBuf);
        return color;
    }

    public void setShadowMapBias(float bias) {
        nSetShadowMapBias(getRef(), bias);
    }

    public float getShadowMapBias () {
        return nGetShadowMapBias(getRef());
    }

    public void setCastShadow (boolean castShadow) {
        nSetCastShadow(getRef(), castShadow);
    }
    public boolean isCastShadow () {
        return nIsCastShadow(getRef());
    }
}



