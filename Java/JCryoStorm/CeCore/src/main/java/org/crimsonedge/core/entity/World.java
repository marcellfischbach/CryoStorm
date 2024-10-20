package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cs::csWorld")
public class csWorld extends CoreObject {

    public csWorld() {
    }

    public csWorld(long ref) {
        super(ref);
    }

    public void log () {
        System.out.println("Log");
    }

    private static native void nAttach (long thisRef, long entityRef);
    public void attach (csEntity entity) {
        nAttach(getRef(), entity.getRef());
    }

    public void detach (csEntity entity) {

    }

}
