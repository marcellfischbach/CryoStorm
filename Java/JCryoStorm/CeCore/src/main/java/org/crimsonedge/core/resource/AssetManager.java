package org.crimsonedge.core.resource;

import org.crimsonedge.core.CeClass;

public class csAssetManager {


    private static native csObject nGet(String cls, String locator);
    private static native csObject nLoad(String cls, String locator);

    public static <T> T get(csClass<T> cls, String locator) {
        CeClass ceCls = cls.getAnnotation(CeClass.class);
        if (ceCls == null) {
            return null;
        }
        csObject obj = nGet(ceCls.value(), locator);
        if (!cls.isInstance(obj)) {
            return null;
        }
        return cls.cast(obj);
    }



    public static <T> T load(csClass<T> cls, String locator) {
        CeClass ceCls = cls.getAnnotation(CeClass.class);
        if (ceCls == null) {
            return null;
        }
        csObject obj = nLoad(ceCls.value(), locator);
        if (!cls.isInstance(obj)) {
            return null;
        }
        return cls.cast(obj);
    }

}
