package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cs::csEntityState")
public abstract class csEntityState extends CoreObject  {


    public csEntityState() {
    }

    public csEntityState(long ref) {
        super(ref);
    }

    private static native void nSetName (long ref, String name);
    private static native String nGetName (long ref);

    private static native void nSetEntity (long ref, long entityRef);
    private static native csEntity nGetEntity (long ref);

    public void setName (String name) {
        nSetName(getRef(), name);
    }

    public String getName () {
        return nGetName(getRef());
    }

    public void setEntity (csEntity entity) {
        nSetEntity(getRef(), entity != null ? entity.getRef() : 0);
    }

    public csEntity getEntity () {
        return nGetEntity(getRef());
    }

}
