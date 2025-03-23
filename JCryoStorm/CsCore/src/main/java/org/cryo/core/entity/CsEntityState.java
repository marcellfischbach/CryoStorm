package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.entity.CsEntityStateNative.*;

@CsClass(CS_CLASS_NAME)
public class CsEntityState extends CsObject {

    public CsEntityState() {
        super();
    }

    public CsEntityState(long ref) {
        super(ref);
    }

    public void setName(String name) {
        nSetName(getRef(), name);
    }

    public String getName() {
        return nGetName(getRef());
    }

    public void setEntity(CsEntity entity) {
        nSetEntity(getRef(), entity != null ? entity.getRef() : 0);
    }

    public CsEntity getEntity() {
        return (CsEntity) nGetEntity(getRef());
    }

    public void setNeedUpdate(boolean needUpdate) {
        nSetNeedUpdate(getRef(), needUpdate);
    }

    public boolean isNeedUpdate() {
        return nIsNeedUpdate(getRef());
    }

    public CsSpatialState getRoot () {
        return (CsSpatialState) nGetRoot(getRef());
    }

}



