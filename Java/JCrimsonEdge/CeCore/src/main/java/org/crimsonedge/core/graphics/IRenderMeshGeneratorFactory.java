package org.crimsonedge.core.graphics;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cs::iRenderMeshGeneratorFactory")
public class IRenderMeshGeneratorFactory extends CoreObject {


    public IRenderMeshGeneratorFactory(long ref) {
        super(ref);
    }


    private static native IRenderMeshGenerator nCreate (long ref);

    public IRenderMeshGenerator create () {
        return nCreate(getRef());
    }
}
