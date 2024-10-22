package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;
import static org.cryo.core.entity.WorldNative.*;

@CsClass(CS_CLASS_NAME)
public class World extends CsObject {

    public World() {
        super();
    }

    public World(long ref) {
        super(ref);
    }

    public void attach(Entity entity) {
        nAttach(getRef(), entity.getRef());
    }


    public void setMainCamera (CameraState camera) {
        nSetMainCamera(getRef(), camera.getRef());
    }
}



