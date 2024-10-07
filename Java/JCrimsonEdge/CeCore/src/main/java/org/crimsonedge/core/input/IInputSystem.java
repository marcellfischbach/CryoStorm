package org.crimsonedge.core.input;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

@CeClass("cs::iInputSystem")
public class IInputSystem extends CoreObject {


    public IInputSystem() {
    }

    public IInputSystem(long ref) {
        super(ref);
    }

    private static native IKeyboard nGetKeyboard(long ref);
    private static native IMouse nGetMouse(long ref);

    public IKeyboard getKeyboard () {
        return nGetKeyboard(getRef());
    }

    public IMouse getMouse () {
        return nGetMouse(getRef()) ;
    }

}
