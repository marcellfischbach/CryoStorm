package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

@CsClass(EntityNative.CS_CLASS_NAME)
public class Entity extends CsObject {

    public Entity() {
        super();
    }

    public Entity(long ref) {
        super(ref);
    }


    public void setName(String name) {
        EntityNative.nSetName(getRef(), name);
    }

    public void attachState(EntityState state) {
        EntityNative.nAttachState(getRef(), state.getRef());
    }

    public void detachState(EntityState state) {
        EntityNative.nDetachState(getRef(), state.getRef());
    }

}



