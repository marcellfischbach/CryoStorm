package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.entity.EntityStateNative.*;

@CsClass(CS_CLASS_NAME)
public class EntityState extends CsObject {

    public EntityState() {
        super();
    }

    public EntityState(long ref) {
        super(ref);
    }

    public void setName(String name) {
        nSetName(getRef(), name);
    }

    public String getName() {
        return nGetName(getRef());
    }

    public void setEntity(Entity entity) {
        nSetEntity(getRef(), entity != null ? entity.getRef() : 0);
    }

    public Entity getEntity() {
        return (Entity) nGetEntity(getRef());
    }

    public void setNeedUpdate(boolean needUpdate) {
        nSetNeedUpdate(getRef(), needUpdate);
    }

    public boolean isNeedUpdate() {
        return nIsNeedUpdate(getRef());
    }

    public SpatialState getRoot () {
        return (SpatialState) nGetRoot(getRef());
    }
}



