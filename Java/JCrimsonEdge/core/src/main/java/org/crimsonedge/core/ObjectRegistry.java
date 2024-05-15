package org.crimsonedge.core;

public class ObjectRegistry {

    private static native Object nGet(String cls);

    public static <T> T get (Class<T> cls) {
        CeClass ceClass = cls.getAnnotation(CeClass.class);
        if (ceClass == null) {
            return null;
        }

        Object obj = nGet(ceClass.value());

        return cls.isInstance(obj) ? cls.cast(obj) : null;
    }
}
