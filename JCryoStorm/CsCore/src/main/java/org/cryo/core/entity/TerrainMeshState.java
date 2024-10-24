package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.entity.TerrainMeshStateNative.*;

@CsClass(CS_CLASS_NAME)
public class TerrainMeshState extends SpatialState {

    public TerrainMeshState() {
    }

    public TerrainMeshState(long ref) {
        super(ref);
    }
}
