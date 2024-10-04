package org.crimsonedge.core.resource;

import org.crimsonedge.core.CeClass;

public class csAssetManager {


    private static native Object nGet(String cls, String locator);
    private static native Object nLoad(String cls, String locator);

    public static <T> T get(Class<T> cls, String locator) {
        CeClass ceCls = cls.getAnnotation(CeClass.class);
        if (ceCls == null) {
            return null;
        }
        Object obj = nGet(ceCls.value(), locator);
        if (!cls.isInstance(obj)) {
            return null;
        }
        return cls.cast(obj);
    }



    public static <T> T load(Class<T> cls, String locator) {
        CeClass ceCls = cls.getAnnotation(CeClass.class);
        if (ceCls == null) {
            return null;
        }
        Object obj = nLoad(ceCls.value(), locator);
        if (!cls.isInstance(obj)) {
            return null;
        }
        return cls.cast(obj);
    }

}
