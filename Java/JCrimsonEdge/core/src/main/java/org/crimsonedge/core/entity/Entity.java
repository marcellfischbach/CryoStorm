package org.crimsonedge.core.entity;

import org.crimsonedge.core.CoreObject;

public class Entity extends CoreObject {

    private static native boolean nAttachEntity (long thisRef, long childRef);
    private static native boolean nDetachEntity (long thisRef, long childRef);
    private static native boolean nAttachState (long thisRef, long stateRef);
    private static native boolean nDetachState (long thisRef, long stateRef);

    public Entity() {
        super("ce::Entity");
    }

    public Entity(long ref) {
        super(ref);
    }

    public boolean attach (Entity child) {
        return nAttachEntity(getRef(), child.getRef());
    }

    public boolean detach (Entity child) {
        return nDetachEntity(getRef(), child.getRef());
    }

    public boolean attach (EntityState child) {
        return nAttachState(getRef(), child.getRef());
    }

    public boolean detach (EntityState child) {
        return nDetachState(getRef(), child.getRef());
    }
}
