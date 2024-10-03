package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.graphics.csMesh;
import org.crimsonedge.core.graphics.material.IMaterial;
import org.crimsonedge.core.graphics.material.csMaterial;

@CeClass("cryo::csStaticMeshState")
public class csStaticMeshState extends csSpatialState {

    public csStaticMeshState() {
    }

    public csStaticMeshState(long ref) {
        super(ref);
    }

    private static native void nSetMesh(long ref, long meshRef);

    private static native csMesh nGetMesh(long ref);

    private static native void nSetMaterial(long ref, int idx, long materialRef);

    private static native IMaterial nGetMaterial(long ref, int idx);


    private static native void nSetReceiveShadow(long ref, boolean castShadow);

    private static native boolean nIsReceiveShadow(long ref);

    private static native void nSetCastShadow(long ref, boolean castShadow);

    private static native boolean nIsCastShadow(long ref);

    public void setMesh(csMesh mesh) {
        nSetMesh(getRef(), mesh != null ? mesh.getRef() : 0);
    }

    public csMesh getMesh() {
        return nGetMesh(getRef());
    }

    public void setMaterial(int idx, IMaterial material) {
        nSetMaterial(getRef(), idx, material != null ? material.getRef() : 0);
    }

    public IMaterial getMaterial(int idx) {
        return nGetMaterial(getRef(), idx);
    }

    public void setReceiveShadow(boolean castShadow) {
        nSetReceiveShadow(getRef(), castShadow);
    }

    public boolean isReceiveShadow() {
        return nIsReceiveShadow(getRef());
    }
    

    public void setCastShadow(boolean castShadow) {
        nSetCastShadow(getRef(), castShadow);
    }

    public boolean isCastShadow() {
        return nIsCastShadow(getRef());
    }
}
