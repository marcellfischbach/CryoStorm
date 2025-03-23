package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import static org.cryo.core.entity.CsWorldNative.*;

@CsClass(CS_CLASS_NAME)
public class CsWorld extends CsObject {

    public CsWorld() {
        super();
    }

    public CsWorld(long ref) {
        super(ref);
    }

    public void attach(CsEntity entity) {
        nAttach(getRef(), entity.getRef());
    }


    public void setMainCamera (CsCameraState camera) {
        nSetMainCamera(getRef(), camera.getRef());
    }
}



