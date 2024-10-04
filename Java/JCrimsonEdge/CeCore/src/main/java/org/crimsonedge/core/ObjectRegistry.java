package org.crimsonedge.core;

public class csObjectRegistry {

    private static native csObject nGet(String cls);

    public static <T> T get (csClass<T> cls) {
        CeClass ceClass = cls.getAnnotation(CeClass.class);
        if (ceClass == null) {
            return null;
        }

        csObject obj = nGet(ceClass.value());

        return cls.isInstance(obj) ? cls.cast(obj) : null;
    }
}
