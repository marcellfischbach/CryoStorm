package org.crimsonedge.core.graphics.material;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.graphics.EFillMode;
import org.crimsonedge.core.graphics.ERenderQueue;
import org.crimsonedge.core.graphics.EShadingMode;
import org.crimsonedge.core.graphics.ITexture;
import org.crimsonedge.core.math.*;

@CeClass("cs::csMaterialInstance")
public class csMaterialInstance extends CoreObject implements IMaterial {

    public csMaterialInstance() {
    }

    public csMaterialInstance(long ref) {
        super(ref);
    }

    private static native void nSetMaterial(long ref, long materialRef);
    private static native csMaterial nGetMaterial(long ref);

    private static native int nGetFillMode(long ref);

    private static native int nGetRenderQueue(long ref);

    private static native int nGetShadingMode(long ref);

    private static native int nIndexOf(long ref, String attributeName);

    private static native void nSetFloat(long ref, int idx, float value);

    private static native void nSetVec2f(long ref, int idx, float x, float y);

    private static native void nSetVec3f(long ref, int idx, float x, float y, float z);

    private static native void nSetVec4f(long ref, int idx, float x, float y, float z, float w);

    private static native void nSetColor4f(long ref, int idx, float x, float y, float z, float w);

    private static native void nSetInt(long ref, int idx, int value);

    private static native void nSetMatrix3f(long ref, int idx, float[] m);
    private static native void nSetMatrix4f(long ref, int idx, float[] m);
    private static native void nSetTexture(long ref, int idx, long textureRef);

    public void setMaterial(csMaterial material) {
        nSetMaterial(getRef(), material != null ? material.getRef() : 0);
    }
    public csMaterial getMaterial() {
        return nGetMaterial(getRef());
    }

    @Override
    public EFillMode getFillMode() {
        return EFillMode.values()[nGetFillMode(getRef())];
    }


    @Override
    public ERenderQueue getRenderQueue() {
        return ERenderQueue.values()[nGetRenderQueue(getRef())];
    }


    @Override
    public EShadingMode getShadingMode() {
        return EShadingMode.values()[nGetShadingMode(getRef())];
    }


    @Override
    public int indexOf(String attributeName) {
        return nIndexOf(getRef(), attributeName);
    }


    @Override
    public void set(int idx, float value) {
        nSetFloat(getRef(), idx, value);
    }


    @Override
    public void set(int idx, csVector2f v) {
        nSetVec2f(getRef(), idx, v.x, v.y);
    }


    @Override
    public void set(int idx, csVector3f v) {
        nSetVec3f(getRef(), idx, v.x, v.y, v.z);
    }


    @Override
    public void set(int idx, csVector4f v) {
        nSetVec4f(getRef(), idx, v.x, v.y, v.z, v.w);
    }


    @Override
    public void set(int idx, csColor4f v) {
        nSetColor4f(getRef(), idx, v.r, v.g, v.b, v.a);
    }


    @Override
    public void set(int idx, int value) {
        nSetInt(getRef(), idx, value);
    }


    private static final float[] F9 = new float[9];

    @Override
    public void set(int idx, csMatrix3f m) {
        m.toArray(F9);
        nSetMatrix3f(getRef(), idx, F9);
    }


    private static final float[] F16 = new float[16];

    @Override
    public void set(int idx, csMatrix4f m) {
        m.toArray(F16);
        nSetMatrix4f(getRef(), idx, F16);

    }

    @Override
    public void set(int idx, ITexture texture) {
        nSetTexture(getRef(), idx, texture != null ? texture.getRef() : 0L);
    }
}
