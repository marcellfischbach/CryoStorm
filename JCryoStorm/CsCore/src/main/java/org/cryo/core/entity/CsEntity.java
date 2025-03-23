package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.entity.CsEntityNative.*;

@CsClass(CS_CLASS_NAME)
public class CsEntity extends CsObject {

    public CsEntity() {
        super();
    }

    public CsEntity(long ref) {
        super(ref);
    }


    public void setName(String name) {
        nSetName(getRef(), name);
    }

    public void attachEntity (CsEntity entity)
    {
        nAttachEntity(getRef(), refOf(entity), 0);
    }

    public void attachEntity (CsEntity entity, CsEntityState state)
    {
        nAttachEntity(getRef(), refOf(entity), refOf(state));
    }

    public void detachEntity (CsEntity entity)
    {
        nDetachEntity(getRef(), refOf(entity));
    }

    public void attachState(CsEntityState state) {
        nAttachState(getRef(), state.getRef());
    }

    public void detachState(CsEntityState state) {
        nDetachState(getRef(), state.getRef());
    }

    public CsSpatialState getRoot ()
    {
        return (CsSpatialState) nGetRoot(getRef());
    }

}



