package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cryo::World")
public class World extends CoreObject {

    public World() {
    }

    public World(long ref) {
        super(ref);
    }

    public void log () {
        System.out.println("Log");
    }

    private static native void nAttach (long thisRef, long entityRef);
    public void attach (Entity entity) {
        nAttach(getRef(), entity.getRef());
    }

    public void detach (Entity entity) {

    }

}
