package org.cryo.core;

@CsClass(JavaModuleNative.CS_CLASS_NAME)
public abstract class JavaModule extends CsObject{

    public JavaModule() {
    }

    public abstract boolean register (String[] args, Engine engine);

    public abstract boolean initialize (String[] args, Engine engine);

}
