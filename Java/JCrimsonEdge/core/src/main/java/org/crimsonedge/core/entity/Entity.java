package org.crimsonedge.core.entity;

import org.crimsonedge.core.CoreObject;

public class Entity extends CoreObject {

    public static native void doSomething ();

    public Entity() {
        super("ce::Entity");
    }

    public Entity(long ref) {
        super(ref);
    }
}
