package org.crimsonedge.core.graphics;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.graphics.material.IMaterial;

@CeClass("cryo::Mesh")
public class Mesh extends CoreObject {

    public Mesh() {
    }

    public Mesh(long ref) {
        super(ref);
    }

    private static native int nAddMaterialSlot(long ref, String name, long materialRef);

    private static native void nSetDefaultMaterial(long ref, int idx, long defaultMaterialRef);

    private static native int nGetNumberOfMaterialSlots(long ref);

    private static native int nIndexOfMaterialSlot(long ref, String name);

    private static native void nAddSubMesh (long ref, long meshRef, int materialSlotIdx);


    public int addMaterialSlot(String name, IMaterial defaultMaterial) {
        return nAddMaterialSlot(getRef(), name, defaultMaterial != null ? defaultMaterial.getRef() : 0);
    }

    public void setDefaultMaterial(int idx, IMaterial defaultMaterial) {
        nSetDefaultMaterial(getRef(), idx, defaultMaterial != null ? defaultMaterial.getRef() : 0);
    }

    public int getNumberOfMaterialSlots() {
        return nGetNumberOfMaterialSlots(getRef());
    }

    public int indexOfMaterialSlot(String name) {
        return nIndexOfMaterialSlot(getRef(), name);
    }

    public void addSubMesh(IRenderMesh mesh, int materialSlotIdx) {
        nAddSubMesh(getRef(), mesh != null ? mesh.getRef() : 0, materialSlotIdx);
    }


}
