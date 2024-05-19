package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("ce::EntityState")
public abstract class EntityState extends CoreObject  {


    public EntityState() {
    }

    public EntityState(long ref) {
        super(ref);
    }

    private static native void nSetName (long ref, String name);
    private static native String nGetName (long ref);

    private static native void nSetEntity (long ref, long entityRef);
    private static native Entity nGetEntity (long ref);

    public void setName (String name) {
        nSetName(getRef(), name);
    }

    public String getName () {
        return nGetName(getRef());
    }

    public void setEntity (Entity entity) {
        nSetEntity(getRef(), entity != null ? entity.getRef() : 0);
    }

    public Entity getEntity () {
        return nGetEntity(getRef());
    }

}
