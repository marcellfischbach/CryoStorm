package org.cryo.core.resource;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import org.cryo.core.ICsObject;

public class AssetManager {

    private long ref;

    private AssetManager () {

    }

    private static native Object nGet(long ref, String cls, String locator);

    private static native Object nLoad(long ref, String cls, String locator);

    public <T extends ICsObject> T get (Class<T> cls, String locator) {
        CsClass csClass = CsObject.getCsClass(cls);
        if (csClass == null) {
            return null;
        }

        Object obj = nGet(ref, csClass.value(), locator);
        return cls.isInstance(obj) ? cls.cast(obj) : null;
    }

    public <T extends ICsObject> T load (Class<T> cls, String locator) {
        CsClass csClass = CsObject.getCsClass(cls);
        if (csClass == null) {
            return null;
        }

        Object obj = nLoad(ref, csClass.value(), locator);
        return cls.isInstance(obj) ? cls.cast(obj) : null;
    }


    private static native long nGet();

    private static AssetManager instance = null;

    public static AssetManager instance () {
        if (instance == null) {
            instance = new AssetManager();
        }
        instance.ref = nGet();
        return instance;
    }


}
