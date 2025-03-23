package org.cryo.core.graphics.material;

import org.cryo.core.CsClass;
import org.cryo.core.graphics.*;
import org.cryo.core.graphics.shading.IShader;
import org.cryo.core.math.CsColor4f;
import org.cryo.core.math.CsVector2f;
import org.cryo.core.math.CsVector3f;
import org.cryo.core.math.CsVector4f;


import static org.cryo.core.graphics.material.CsMaterialNative.*;

@CsClass(CS_CLASS_NAME)
public class CsMaterial extends IMaterial {

    public CsMaterial() {
        super();
    }

    public CsMaterial(long ref) {
        super(ref);
    }


    public boolean bind(IDevice device, ERenderPass pass) {
        return nBind(getRef(), device.getRef(), pass.ordinal());
    }

    public void setFillMode (EFillMode fillMode) {
        nSetFillMode(getRef(), fillMode.ordinal());
    }

    public EFillMode getFillMode() {
        return EFillMode.values()[nGetFillMode(getRef())];
    }

    public void setRenderQueue (ERenderQueue renderQueue) {
        nSetRenderQueue(getRef(), renderQueue.ordinal());
    }

    public ERenderQueue getRenderQueue() {
        return ERenderQueue.values()[nGetRenderQueue(getRef())];
    }

    public void setBlending (boolean blending) {
        nSetBlending(getRef(), blending);
    }

    public boolean isBlending() {
        return nIsBlending(getRef());
    }
    
    public void setBlendFactor (EBlendFactor srcFactor, EBlendFactor dstFactor) {
        nSetBlendFactor(getRef(), srcFactor.ordinal(), dstFactor.ordinal());
    }
    
    public void setBlendFactor(EBlendFactor srcFactorColor,
                               EBlendFactor srcFactorAlpha,
                               EBlendFactor dstFactorColor,
                               EBlendFactor dstFactorAlpha) {
        nSetBlendFactor(getRef(),
                        srcFactorColor.ordinal(),
                        srcFactorAlpha.ordinal(),
                        dstFactorColor.ordinal(),
                        dstFactorAlpha.ordinal());
    }
    
    public EBlendFactor getBlendFactorSrcColor () {
        return EBlendFactor.values()[nGetBlendFactorSrcColor(getRef())];
    }

    public EBlendFactor getBlendFactorSrcAlpha () {
        return EBlendFactor.values()[nGetBlendFactorSrcAlpha(getRef())];
    }

    public EBlendFactor getBlendFactorDstColor () {
        return EBlendFactor.values()[nGetBlendFactorDstColor(getRef())];
    }

    public EBlendFactor getBlendFactorDstAlpha () {
        return EBlendFactor.values()[nGetBlendFactorDstAlpha(getRef())];
    }

    public void setDepthWrite (boolean depthWrite) {
        nSetDepthWrite(getRef(), depthWrite);
    }

    public boolean isDepthWrite() {
        return nIsDepthWrite(getRef());
    }

    public void setDepthTest (boolean depthTest) {
        nSetDepthTest(getRef(), depthTest);
    }

    public boolean isDepthTest() {
        return nIsDepthTest(getRef());
    }

    public void setShadingMode (EShadingMode shadingMode) {
        nSetShadingMode(getRef(), shadingMode.ordinal());
    }

    public EShadingMode getShadingMode() {
        return EShadingMode.values()[nGetShadingMode(getRef())];
    }

    public void setShader(ERenderQueue pass, IShader shader) {
        nSetShader(getRef(), pass.ordinal(), shader.getRef());
    }

    public IShader getShader(ERenderPass pass) {
        return (IShader) nGetShader(getRef(), pass.ordinal());
    }

    public void registerAttribute (String attributeName, EMaterialAttributeType attributeType) {
        nRegisterAttribute(getRef(), attributeName, attributeType.ordinal());
    }

    public long getNumberOfAttributes () {
        return nGetNumberOfAttributes(getRef());
    }

    public String[] getAttributeNames () {
        return nGetAttributeNames(getRef());
    }

    public long indexOf (String attributeName) {
        return nIndexOf(getRef(), attributeName);
    }

    public void setFloat (long idx, float val) {
        nSetFloat(getRef(), idx, val);
    }

    public void setVector2f(long idx, CsVector2f v) {
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



