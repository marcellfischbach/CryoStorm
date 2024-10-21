package org.cryo.core;

@CsClass(EngineNative.CS_CLASS_NAME)
public class Engine extends CsObject{

    public Engine(long ref) {
        super(ref);
    }


    public void initializeEngine(String[] args, ModuleConfig moduleConfig) {
        EngineNative.nInitializeEngine(getRef(), args, moduleConfig.getRef());
    }

    private static native Object nGet();
    public static Engine get() {
        return (Engine)nGet();
    }
}
