package org.cryo.core.entity;

import org.cryo.core.CoreObject;
import org.cryo.core.CsClass;
import static org.cryo.core.entity.EntityNative.*;

@CsClass("cs::csEntity")
public class Entity extends CoreObject {

    public Entity() {
    }

    public Entity(long ref) {
        super(ref);
    }

    public boolean attach (EntityState state) {
        return nAttach(getRef(), state.getRef());
    }

    public boolean detach (EntityState state) {
        return nDetach(getRef(), state.getRef());
    }
}
