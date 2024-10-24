package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.entity.TerrainLayerNative.*;

@CsClass(CS_CLASS_NAME)
public class TerrainLayer extends CsObject {

    public TerrainLayer() {
    }

    public TerrainLayer(long ref) {
        super(ref);
    }
}
