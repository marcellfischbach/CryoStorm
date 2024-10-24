package org.cryo.core.graphics;

import org.cryo.core.CsClass;
import org.cryo.core.math.Vector3f;

import static org.cryo.core.graphics.ITerrainMeshNative.*;

@CsClass(CS_CLASS_NAME)
public class ITerrainMesh extends IRenderMesh {
    public ITerrainMesh(long ref) {
        super(ref);
    }

    public void setReferencePoint (Vector3f pos) {
        nSetReferencePoint(getRef(), pos.x, pos.y, pos.z);
    }
}
