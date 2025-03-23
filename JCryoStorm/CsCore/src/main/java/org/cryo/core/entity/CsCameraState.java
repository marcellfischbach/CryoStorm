package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.EClearColorMode;
import org.cryo.core.graphics.EClearMode;
import org.cryo.core.math.CsColor4f;

import static org.cryo.core.entity.CsCameraStateNative.*;

@CsClass(CS_CLASS_NAME)
public class CsCameraState extends CsSpatialState
{

    public CsCameraState() {
    }

    public CsCameraState(long ref) {
        super(ref);
    }


    public void setNear (float near) {
        nSetNear(getRef(), near);
    }

    public float getNear () {
        return nGetNear(getRef());
    }

    public void setFar (float far) {
        nSetFar(getRef(), far);
    }

    public float getFar () {
        return nGetFar(getRef());
    }

    public void setAngle (float angle) {
        nSetAngle(getRef(), angle);
    }

    public float getAngle () {
        return nGetAngle(getRef());
    }

    public void setAngleWidthHeight(float angleWidthHeight) {
        nSetAngleWidthHeight(getRef(), angleWidthHeight);
    }

    public float getAngleWidthHeight () {
        return nGetAngleWidthHeight(getRef());
    }

    public void setOrder (int order) {
        nSetOrder(getRef(), order);
    }

    public int getOrder () {
        return nGetOrder(getRef());
    }

    public void setClearMode (EClearMode clearMode) {
        nSetClearMode(getRef(), clearMode.ordinal());
    }

    public EClearMode getClearMode () {
        return EClearMode.values()[nGetClearMode(getRef())];
    }

    public void setClearColorMode(EClearColorMode clearColorMode) {
        nSetClearColorMode(getRef(), clearColorMode.ordinal());
    }

    public EClearColorMode getClearColorMode () {
        return EClearColorMode.values()[nGetClearColorMode(getRef())];
    }

    public void setClearColor (CsColor4f clearColor) {
        nSetClearColor(getRef(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }

    private final float[] clearColorMem = new float[4];
    private final CsColor4f clearColor = new CsColor4f();
    public CsColor4f getClearColor () {
        nGetClearColor(getRef(), clearColorMem);
        return clearColor.set(clearColorMem);
    }

    public void setClearDepth (float clearDepth) {
        nSetClearDepth(getRef(), clearDepth);
    }

    public float getClearDepth () {
        return nGetClearDepth(getRef());
    }

    public void setRenderShadows (boolean renderShadows) {
        nSetRenderShadows(getRef(), renderShadows);
    }

    public boolean isRenderShadows () {
        return nIsRenderShadows(getRef());
    }

    public void update (long width, long height) {
        nUpdate(getRef(), width, height);
    }
}
