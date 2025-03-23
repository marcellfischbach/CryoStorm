package org.cryo.core;

import static org.cryo.core.CsEngineNative.*;

@CsClass(CS_CLASS_NAME)
public class CsEngine extends CsObject{

    public CsEngine(long ref) {
        super(ref);
    }


    public void initializeEngine(String[] args, CsModuleConfig moduleConfig) {
        nInitializeEngine(getRef(), args, moduleConfig.getRef());
    }

    private static native Object nGet();
    public static CsEngine get() {
        return (CsEngine)nGet();
    }

}
