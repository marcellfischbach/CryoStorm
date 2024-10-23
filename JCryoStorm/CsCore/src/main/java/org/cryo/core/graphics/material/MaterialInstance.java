package org.cryo.core.graphics.material;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import org.cryo.core.graphics.*;
import org.cryo.core.graphics.shading.IShader;
import org.cryo.core.math.Color4f;
import org.cryo.core.math.Vector2f;
import org.cryo.core.math.Vector3f;
import org.cryo.core.math.Vector4f;

import static org.cryo.core.graphics.material.MaterialInstanceNative.*;

@CsClass(CS_CLASS_NAME)
public class MaterialInstance extends IMaterial {

    public MaterialInstance() {
        super();
    }

    public MaterialInstance(long ref) {
        super(ref);
    }

    public IShader getShader(ERenderPass pass) {
        return (IShader) nGetShader(getRef(), pass.ordinal());
    }


    public EFillMode getFillMode() {
        return EFillMode.values()[nGetFillMode(getRef())];
    }

    public ERenderQueue getRenderQueue() {
        return ERenderQueue.values()[nGetRenderQueue(getRef())];
    }

    public EShadingMode getShadingMode() {
        return EShadingMode.values()[nGetShadingMode(getRef())];
    }

    public boolean bind(IDevice device, ERenderPass pass) {
        return nBind(getRef(), device.getRef(), pass.ordinal());
    }

    public void setMaterial (Material material) {
        nSetMaterial(getRef(), material.getRef());
    }

    public Material getMaterial () {
        return (Material)nGetMaterial(getRef());
    }

    public long indexOf (String attributeName) {
        return nIndexOf(getRef(), attributeName);
    }

    public void setFloat (long idx, float val) {
        nSetFloat(getRef(), idx, val);
    }

    public void setVector2f(long idx, Vector2f v) {
        nSetVector2f(getRef(), idx, v.x, v.y);
    }

    public void setVector3f(long idx, Vector3f v) {
        nSetVector3f(getRef(), idx, v.x, v.y, v.z);
    }

    public void setVector4f(long idx, Vector4f v) {
        nSetVector4f(getRef(), idx, v.x, v.y, v.z, v.w);
    }

    public void setColor4f(long idx, Color4f v) {
        nSetColor4f(getRef(), idx, v.r, v.g, v.b, v.a);
    }

    public void setTexture (long idx, ITexture texture) {
        nSetTexture(getRef(), idx, texture.getRef());
    }

    public boolean isOverridden (long idx) {
        return nIsOverridden(getRef(), idx);
    }

    public void setOverride (long idx, boolean override) {
        nSetOverride(getRef(), idx, override);
    }
}



