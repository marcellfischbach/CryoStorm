package org.cryo.core.graphics;

import org.cryo.core.CsClass;
import org.cryo.core.math.Vector3f;

import static org.cryo.core.graphics.ITerrainMeshGeneratorNative.*;

@CsClass(CS_CLASS_NAME)
public class ITerrainMeshGenerator extends IRenderMesh {
    public ITerrainMeshGenerator(long ref) {
        super(ref);
    }

    public void setSize (ETerrainSize size) {
        nSetSize(getRef(), size.size);
    }

    public void setPatchSize (ETerrainSize size) {
        nSetPatchSize(getRef(), size.size);
    }

    public void setSize (Vector3f min, Vector3f max) {
        nSetSize(getRef(), min.x, min.y, min.z, max.x, max.y, max.z);
    }

    public void setNormalizedHeightData (float[] heightData) {
        nSetNormalizedHeightData(getRef(), heightData);
    }

    public void setHeightData (float[] heightData) {
        nSetHeightData(getRef(), heightData);
    }

    public ITerrainMesh generate () {
        return (ITerrainMesh) nGenerate(getRef());
    }
}
