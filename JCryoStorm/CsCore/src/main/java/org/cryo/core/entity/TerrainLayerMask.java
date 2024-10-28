package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import org.cryo.core.graphics.ITexture2D;

import static org.cryo.core.entity.TerrainLayerMaskNative.*;

@CsClass(CS_CLASS_NAME)
public class TerrainLayerMask extends CsObject {

    public TerrainLayerMask() {
    }

    public TerrainLayerMask(long ref) {
        super(ref);
    }

    public void setLayerTexture(ITexture2D layerTexture) {
        nSetLayerTexture(getRef(), layerTexture != null ? layerTexture.getRef() : 0);
    }

    public ITexture2D getLayerTexture() {
        return (ITexture2D) nGetLayerTexture(getRef());
    }

    public void setMaskTexture(ITexture2D maskTexture) {
        nSetMaskTexture(getRef(), maskTexture != null ? maskTexture.getRef() : 0);
    }

    public ITexture2D getMaskTexture() {
        return (ITexture2D) nGetMaskTexture(getRef());
    }
}
