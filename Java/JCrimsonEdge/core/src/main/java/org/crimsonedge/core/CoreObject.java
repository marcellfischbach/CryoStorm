package org.crimsonedge.core;

public class CoreObject {

    private long ref;

    private native long nCreateClass (String className);

    public CoreObject(String cClassName) {
        ref = nCreateClass(cClassName);
    }

    public CoreObject(long ref) {
        this.ref = ref;
    }

    public long getRef() {
        return ref;
    }

}
