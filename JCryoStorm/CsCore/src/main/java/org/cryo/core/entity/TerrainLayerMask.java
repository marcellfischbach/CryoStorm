package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.entity.TerrainLayerMaskNative.CS_CLASS_NAME;

@CsClass(CS_CLASS_NAME)
public class TerrainLayerMask extends CsObject {

    public TerrainLayerMask() {
    }

    public TerrainLayerMask(long ref) {
        super(ref);
    }
}
