package org.cryo.core;

public class ObjectRegistry {

    private static native void nRegister (String clsName, long ref);
    private static native Object nGet (String clsName);

    public static void register (Class<? extends ICsObject> cls, ICsObject obj) {
        if (obj == null) {
            throw new NullPointerException();
        }

        CsClass csClass = CsObject.getCsClass(cls);
        if (csClass == null) {
            throw new NoCsClassFoundException(cls.getName());
        }
        nRegister(csClass.value(), obj.getRef());
    }

    public static <T extends ICsObject> T get (Class<T> cls) {
        CsClass csClass = CsObject.getCsClass(cls);
        if (csClass == null) {
            throw new NoCsClassFoundException(cls.getName());
        }

        Object obj = nGet(csClass.value());
        if (obj == null) {
            return null;
        }

        if (!cls.isInstance(obj)) {
            throw new ClassCastException();
        }
        return cls.cast(obj);
    }

}
