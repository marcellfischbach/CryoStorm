package org.cryo.core.entity;

import org.cryo.core.CsClass;
import org.cryo.core.math.Matrix4f;

import static org.cryo.core.entity.SpatialStateNative.*;

@CsClass(CS_CLASS_NAME)
public class SpatialState extends EntityState {

    private final Transform transform = new Transform(this);

    public SpatialState() {
        super();
    }

    public SpatialState(long ref) {
        super(ref);
    }

    public Transform getTransform() {
        SpatialState parent = getParent();
        Matrix4f localMatrix = getLocalMatrix();

        if (parent != null) {
            return transform.set(localMatrix, parent.getGlobalMatrix());
        }
        return transform.set(localMatrix, Matrix4f.IDENTITY);
    }

    public void setStatic(boolean _static) {
        nSetStatic(getRef(), _static);
    }

    public boolean isStatic() {
        return nIsStatic(getRef());
    }


    public void attachSpatial(SpatialState spatialState) {
        nAttachSpatial(getRef(), spatialState.getRef());
    }

    public void detachSelf() {
        nDetachSelf(getRef());
    }

    public void detachSpatial(SpatialState spatialState) {
        nDetachSpatial(getRef(), spatialState.getRef());
    }

    public SpatialState getParent() {
        return (SpatialState) nGetParent(getRef());
    }

    public long getNumberOfChildren() {
        return nGetNumberOfChildren(getRef());
    }

    public SpatialState getChild(long index) {
        return (SpatialState) nGetChild(getRef(), index);
    }


    private final Matrix4f globalMatrix = new Matrix4f();
    private final float[] globalBuffer = new float[16];

    public Matrix4f getGlobalMatrix() {
        nGetGlobalMatrix(getRef(), globalBuffer);
        return globalMatrix.set(globalBuffer);
    }

    private final Matrix4f localMatrix = new Matrix4f();
    private final float[] localBuffer = new float[16];

    public Matrix4f getLocalMatrix() {
        nGetLocalMatrix(getRef(), localBuffer);
        return localMatrix.set(localBuffer);
    }

    public void setLocalMatrix(Matrix4f localMatrix) {
        localMatrix.get(this.localBuffer);
        nSetLocalMatrix(getRef(), this.localBuffer);
    }

    public void updateTransformation () {
        nUpdateTransformation(getRef());
    }
}



