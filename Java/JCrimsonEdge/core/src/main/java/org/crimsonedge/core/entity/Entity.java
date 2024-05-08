package org.crimsonedge.core.entity;

import org.crimsonedge.core.CoreObject;

public class Entity extends CoreObject {

    public static native void doSomething ();

    public Entity() {
    }

    public Entity(long ref) {
        super(ref);
        System.out.println("Entity.Entity(" + ref + ")");
    }
}
