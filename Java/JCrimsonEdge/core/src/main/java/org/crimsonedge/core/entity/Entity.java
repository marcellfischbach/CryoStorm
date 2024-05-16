package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("ce::Entity")
public class Entity extends CoreObject {

    private static native void nSetName(long ref, String name);

    private static native String nGetName(long ref);

    private static native World nGetWorld(long ref);

    private static native boolean nAttachEntity(long ref, long childRef, long parentRef);

    private static native boolean nDetachEntity(long ref, long childRef);

    private static native Entity nGetParent (long ref);

    private static native int nGetNumberOfChildren(long ref);

    private static native Entity nGetChild(long ref, int idx);

    private static native boolean nAttachState(long ref, long stateRef);

    private static native boolean nDetachState(long ref, long stateRef);

    private static native void nSetRoot (long ref, long rootStateRef);

    private static native SpatialState nGetRoot (long ref);

    public Entity() {
    }

    public Entity(long ref) {
        super(ref);
    }

    public void setName(String name) {
        nSetName(getRef(), name);
    }

    public String getName() {
        return nGetName(getRef());
    }

    public World getWorld() {
        return nGetWorld(getRef());
    }

    public boolean attach(Entity child) {
        return attach(child, null);
    }

    public boolean attach(Entity child, SpatialState parentState) {
        return nAttachEntity(getRef(),
                             child != null ? child.getRef() : 0,
                             parentState != null ? parentState.getRef() : 0);
    }


    public boolean detach(Entity child) {
        return nDetachEntity(getRef(), child.getRef());
    }

    public Entity getParent () {
        return nGetParent(getRef());
    }

    public int getNumberOfChildren () {
        return nGetNumberOfChildren(getRef());
    }

    public Entity getChild (int idx) {
        return nGetChild(getRef(), idx);
    }

    public boolean attach(EntityState child) {
        return nAttachState(getRef(), child.getRef());
    }

    public boolean detach(EntityState child) {
        return nDetachState(getRef(), child.getRef());
    }

    public void setRoot(SpatialState rootState) {
        nSetRoot(getRef(), rootState != null ? rootState.getRef() : 0);
    }

    public SpatialState getRoot() {
        return nGetRoot(getRef());
    }
}
