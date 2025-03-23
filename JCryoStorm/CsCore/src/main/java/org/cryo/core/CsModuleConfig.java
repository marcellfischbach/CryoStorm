package org.cryo.core;

import static org.cryo.core.CsModuleConfigNative.*;

@CsClass(CS_CLASS_NAME)
public class CsModuleConfig extends CsObject {
    public CsModuleConfig() {
    }

    public CsModuleConfig(long ref) {
        super(ref);
    }

    public boolean loadModuleConfig () {
        return nLoadModuleConfig(getRef());
    }

    public boolean loadModuleConfigEx (String filename) {
        return nLoadModuleConfigEx(getRef(), filename);
    }

    public void addModule(CsJavaModule module) {
        nAddModule(getRef(), module.getRef());
    }

    public void addModuleByName (String moduleName) {
        nAddModuleByName(getRef(), moduleName);
    }

}
