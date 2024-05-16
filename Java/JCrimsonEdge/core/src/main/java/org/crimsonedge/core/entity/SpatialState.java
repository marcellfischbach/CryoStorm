package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;

@CeClass("ce::SpatialState")
public class SpatialState extends EntityState {

    public SpatialState() {
    }

    public SpatialState(long ref) {
        super(ref);
    }


    private static native void nSetStatic(long ref, boolean _static);

    private static native boolean nIsStatic(long ref);

    private static native boolean nAttach(long ref, long childRef);

    private static native boolean nDetachSelf(long ref);

    private static native boolean nDetach(long ref, long childRef);

    private static native SpatialState nGetParent(long ref);

    private static native int nGetNumberOfChildren(long ref);

    private static native SpatialState nGetChild(long ref, int idx);

    public void setStatic(boolean _static) {
        nSetStatic(getRef(), _static);
    }

    public boolean isStatic() {
        return nIsStatic(getRef());
    }

    public boolean attach(SpatialState child) {
        return nAttach(getRef(), child != null ? child.getRef() : 0);
    }

    public boolean detachSelf() {
        return nDetachSelf(getRef());
    }

    public boolean detach(SpatialState child) {
        return nDetach(getRef(), child != null ? child.getRef() : 0);
    }

    public SpatialState getParent() {
        return nGetParent(getRef());
    }

    public int getNumberOfChildren() {
        return nGetNumberOfChildren(getRef());
    }

    public SpatialState getChild(int idx) {
        return nGetChild(getRef(), idx);
    }
}
