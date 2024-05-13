package org.crimsonedge.core.entity;

import org.crimsonedge.core.CoreObject;

public abstract class EntityState extends CoreObject  {

    public EntityState(String cClassName) {
        super(cClassName);
    }

    public EntityState(long ref) {
        super(ref);
    }
}
