package org.crimsonedge.core;

public class CoreObject implements ICoreObject{

    private long ref;

    private native long nCreateClass (String className);

    public CoreObject() {
        CeClass cls = getClass().getAnnotation(CeClass.class);
        if (cls != null) {
            ref = nCreateClass(cls.value());
        }
        else {
            ref = 0;
        }
    }

    public CoreObject(long ref) {
        this.ref = ref;
    }

    @Override
    public long getRef() {
        return ref;
    }

}
