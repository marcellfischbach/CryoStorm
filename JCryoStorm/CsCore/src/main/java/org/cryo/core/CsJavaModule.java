package org.cryo.core;

import static org.cryo.core.CsJavaModuleNative.*;

@CsClass(CS_CLASS_NAME)
public abstract class CsJavaModule extends CsObject{

    public CsJavaModule() {
    }

    public abstract boolean register (String[] args, CsEngine engine);

    public abstract boolean initialize (String[] args, CsEngine engine);

}
