package org.cryo.core;

public class CsObject implements ICsObject {

    private long ref;

    private static native long nReleaseObject(long ref);
    private static native long nCreateClass(Object obj, String className);

    public CsObject() {
        ref = createRef(this, getClass());
    }

    public CsObject(long ref) {
        this.ref = ref;
    }

    public static long createRef (ICsObject obj, Class<? extends ICsObject> cls) {
        if (obj == null) {
            throw new NullPointerException("ICsObject must not be null");
        }

        CsClass csClass = getCsClass(cls);
        if (csClass != null) {
            return nCreateClass(obj, csClass.value());
        } else {
            throw new CsClassInstantiationException(obj.getClass() + " is not annotated with @CsClass(\"...\")");
        }
    }

    public static CsClass getCsClass(Class<?> cls) {
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
        if (ref == 0) {
            throw new NullPointerException("This object [" + getClass().getName() + "] has no reference");
        }
        return ref;
    }

    public static long refOf(CsObject obj)
    {
        return obj != null ? obj.ref : 0;
    }

}
