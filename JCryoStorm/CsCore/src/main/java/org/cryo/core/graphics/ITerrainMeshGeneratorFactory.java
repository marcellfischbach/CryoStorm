package org.cryo.core.graphics;

import org.cryo.core.CsClass;

import static org.cryo.core.graphics.ITerrainMeshGeneratorFactoryNative.*;

@CsClass(CS_CLASS_NAME)
public class ITerrainMeshGeneratorFactory extends IRenderMesh {
    public ITerrainMeshGeneratorFactory(long ref) {
        super(ref);
    }

    public ITerrainMeshGenerator Create () {
        return (ITerrainMeshGenerator) nCreate(getRef());
    }
}
