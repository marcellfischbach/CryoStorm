package org.cryo.core.graphics.material;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import org.cryo.core.graphics.*;
import org.cryo.core.graphics.shading.IShader;
import org.cryo.core.math.CsColor4f;
import org.cryo.core.math.CsVector2f;
import org.cryo.core.math.CsVector3f;
import org.cryo.core.math.CsVector4f;

import static org.cryo.core.graphics.material.IMaterialNative.*;

@CsClass(CS_CLASS_NAME)
public class IMaterial extends CsObject {

    public IMaterial() {
        super();
    }

    public IMaterial(long ref) {
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

    public boolean bind (IDevice device, ERenderPass pass) {
        return nBind(getRef(), device.getRef(), pass.ordinal());
    }

    public long indexOf (String attributeName) {
        return nIndexOf(getRef(), attributeName);
    }

    public void setFloat (long idx, float val) {
        nSetFloat(getRef(), idx, val);
    }

    public void setVector2f (long idx, CsVector2f v) {
        nSetVector2f(getRef(), idx, v.x, v.y);
    }

    public void setVector3f(long idx, CsVector3f v) {
        nSetVector3f(getRef(), idx, v.x, v.y, v.z);
    }

    public void setVector4f(long idx, CsVector4f v) {
        nSetVector4f(getRef(), idx, v.x, v.y, v.z, v.w);
    }

    public void setColor4f(long idx, CsColor4f v) {
        nSetColor4f(getRef(), idx, v.r, v.g, v.b, v.a);
    }

    public void setTexture (long idx, ITexture texture) {
        nSetTexture(getRef(), idx, texture.getRef());
    }

}



