package org.cryo.core.resource;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import org.cryo.core.ICsObject;

public class CsAssetManager
{

    private long ref;

    private CsAssetManager() {

    }

    private static native Object nGet(long ref, String locator);

    private static native Object nLoad(long ref, String locator);

    public <T extends ICsObject> T get (Class<T> cls, String locator) {
        CsClass csClass = CsObject.getCsClass(cls);
        if (csClass == null) {
            return null;
        }

        Object obj = nGet(ref, locator);
        return cls.isInstance(obj) ? cls.cast(obj) : null;
    }

    public <T extends ICsObject> T load (Class<T> cls, String locator) {
        CsClass csClass = CsObject.getCsClass(cls);
        if (csClass == null) {
            return null;
        }

        Object obj = nLoad(ref, locator);
        return cls.isInstance(obj) ? cls.cast(obj) : null;
    }


    private static native long nGet();

    private static CsAssetManager instance = null;

    public static CsAssetManager instance () {
        if (instance == null) {
            instance = new CsAssetManager();
        }
        instance.ref = nGet();
        return instance;
    }


}
