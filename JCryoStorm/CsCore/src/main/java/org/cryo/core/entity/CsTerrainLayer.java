package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import org.cryo.core.graphics.ITexture2D;

import static org.cryo.core.entity.CsTerrainLayerNative.*;

@CsClass(CS_CLASS_NAME)
public class CsTerrainLayer extends CsObject {

    public CsTerrainLayer() {
    }

    public CsTerrainLayer(long ref) {
        super(ref);
    }

    public void setDiffuseRoughness (ITexture2D diffuseRoughness) {
        nSetDiffuseRoughness(getRef(), diffuseRoughness != null ? diffuseRoughness.getRef() : 0);
    }

    public ITexture2D getDiffuseRoughness () {
        return (ITexture2D) nGetDiffuseRoughness(getRef());
    }

    public void setNormal (ITexture2D normal) {
        nSetNormal(getRef(), normal != null ? normal.getRef() : 0);
    }

    public ITexture2D getNormal () {
        return (ITexture2D) nGetNormal(getRef());
    }
}
