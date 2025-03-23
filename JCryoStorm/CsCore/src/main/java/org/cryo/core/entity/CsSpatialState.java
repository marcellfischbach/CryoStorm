package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.math.CsMatrix4f;

import static org.cryo.core.entity.CsSpatialStateNative.*;

@CsClass(CS_CLASS_NAME)
public class CsSpatialState extends CsEntityState
{

    private final CsTransform transform = new CsTransform(this);

    public CsSpatialState() {
        super();
    }

    public CsSpatialState(long ref) {
        super(ref);
    }

    public CsTransform getTransform() {
        CsSpatialState parent = getParent();
        CsMatrix4f localMatrix = getLocalMatrix();

        if (parent != null) {
            return transform.set(localMatrix, parent.getGlobalMatrix());
        }
        return transform.set(localMatrix, CsMatrix4f.IDENTITY);
    }

    public void setStatic(boolean _static) {
        nSetStatic(getRef(), _static);
    }

    public boolean isStatic() {
        return nIsStatic(getRef());
    }


    public void attachSpatial(CsSpatialState spatialState) {
        nAttachSpatial(getRef(), spatialState.getRef());
    }

    public void detachSelf() {
        nDetachSelf(getRef());
    }

    public void detachSpatial(CsSpatialState spatialState) {
        nDetachSpatial(getRef(), spatialState.getRef());
    }

    public CsSpatialState getParent() {
        return (CsSpatialState) nGetParent(getRef());
    }

    public long getNumberOfChildren() {
        return nGetNumberOfChildren(getRef());
    }

    public CsSpatialState getChild(long index) {
        return (CsSpatialState) nGetChild(getRef(), index);
    }


    private final CsMatrix4f globalMatrix = new CsMatrix4f();
    private final float[] globalBuffer = new float[16];

    public CsMatrix4f getGlobalMatrix() {
        nGetGlobalMatrix(getRef(), globalBuffer);
        return globalMatrix.set(globalBuffer);
    }

    private final CsMatrix4f localMatrix = new CsMatrix4f();
    private final float[] localBuffer = new float[16];

    public CsMatrix4f getLocalMatrix() {
        nGetLocalMatrix(getRef(), localBuffer);
        return localMatrix.set(localBuffer);
    }

    public void setLocalMatrix(CsMatrix4f localMatrix) {
        localMatrix.get(this.localBuffer);
        nSetLocalMatrix(getRef(), this.localBuffer);
    }

    public void updateTransformation () {
        nUpdateTransformation(getRef());
    }
}



