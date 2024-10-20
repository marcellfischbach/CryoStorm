package org.cryo.core;

public class CsObject implements ICoreObject {

    private long ref;

    private native long nCreateClass(String className);

    public CsObject() {
        CsClass cls = getCsClass(getClass());
        if (cls != null) {
            ref = nCreateClass(cls.value());
        } else {
            throw new CsClassInstantiationException(getClass() + " is not annotated with @CsClass(\"...\")");
        }
    }

    public CsObject(long ref) {
        this.ref = ref;
    }

    private static CsClass getCsClass(Class<?> cls) {
        while (cls != null) {
            CsClass ceClass = cls.getAnnotation(CsClass.class);
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
