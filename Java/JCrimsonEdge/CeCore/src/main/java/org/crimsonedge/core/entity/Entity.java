package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cryo::csEntity")
public class csEntity extends CoreObject {

    private static native void nSetName(long ref, String name);

    private static native String nGetName(long ref);

    private static native csWorld nGetWorld(long ref);

    private static native boolean nAttachEntity(long ref, long childRef, long parentRef);

    private static native boolean nDetachEntity(long ref, long childRef);

    private static native csEntity nGetParent (long ref);

    private static native int nGetNumberOfChildren(long ref);

    private static native csEntity nGetChild(long ref, int idx);

    private static native boolean nAttachState(long ref, long stateRef);

    private static native boolean nDetachState(long ref, long stateRef);

    private static native void nSetRoot (long ref, long rootStateRef);

    private static native csSpatialState nGetRoot (long ref);

    public csEntity() {
    }

    public csEntity(long ref) {
        super(ref);
    }

    public void setName(String name) {
        nSetName(getRef(), name);
    }

    public String getName() {
        return nGetName(getRef());
    }

    public csWorld getWorld() {
        return nGetWorld(getRef());
    }

    public boolean attach(csEntity child) {
        return attach(child, null);
    }

    public boolean attach(csEntity child, csSpatialState parentState) {
        return nAttachEntity(getRef(),
                             child != null ? child.getRef() : 0,
                             parentState != null ? parentState.getRef() : 0);
    }


    public boolean detach(csEntity child) {
        return nDetachEntity(getRef(), child.getRef());
    }

    public csEntity getParent () {
        return nGetParent(getRef());
    }

    public int getNumberOfChildren () {
        return nGetNumberOfChildren(getRef());
    }

    public csEntity getChild (int idx) {
        return nGetChild(getRef(), idx);
    }

    public boolean attach(csEntityState child) {
        return nAttachState(getRef(), child.getRef());
    }

    public boolean detach(csEntityState child) {
        return nDetachState(getRef(), child.getRef());
    }

    public void setRoot(csSpatialState rootState) {
        nSetRoot(getRef(), rootState != null ? rootState.getRef() : 0);
    }

    public csSpatialState getRoot() {
        return nGetRoot(getRef());
    }
}
