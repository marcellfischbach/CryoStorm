package org.cryo.core;

@CsClass(ModuleConfigNative.CS_CLASS_NAME)
public class ModuleConfig extends CsObject {
    public ModuleConfig() {
    }

    public ModuleConfig(long ref) {
        super(ref);
    }

    public boolean loadModuleConfig () {
        return ModuleConfigNative.nLoadModuleConfig(getRef());
    }

    public boolean loadModuleConfigEx (String filename) {
        return ModuleConfigNative.nLoadModuleConfigEx(getRef(), filename);
    }

    public void addModule(JavaModule module) {
        ModuleConfigNative.nAddModule(getRef(), module.getRef());
    }

    public void addModuleByName (String moduleName) {
        ModuleConfigNative.nAddModuleByName(getRef(), moduleName);
    }

}
