package org.crimsonedge.core;

public class CoreObject implements ICoreObject{

    private long ref;

    private native long nCreateClass (String className);

    public CoreObject() {
        CeClass cls = getCeClass(getClass());
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

    private static CeClass getCeClass(Class<?> cls) {
        while (cls != null) {
            CeClass ceClass = cls.getAnnotation(CeClass.class);
            if (ceClass != null) {
                return ceClass;
            }

            cls = cls.getSuperclass();
        }
        return null;
    }

    @Override
    public long getRef() {
        return ref;
    }

}
