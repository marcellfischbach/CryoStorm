package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.math.Matrix4f;

@CeClass("cryo::csSpatialState")
public class csSpatialState extends csEntityState {

    public csSpatialState() {
    }

    public csSpatialState(long ref) {
        super(ref);
    }


    private static native void nSetStatic(long ref, boolean _static);

    private static native boolean nIsStatic(long ref);

    private static native boolean nAttach(long ref, long childRef);

    private static native boolean nDetachSelf(long ref);

    private static native boolean nDetach(long ref, long childRef);

    private static native csSpatialState nGetParent(long ref);

    private static native int nGetNumberOfChildren(long ref);

    private static native csSpatialState nGetChild(long ref, int idx);

    private static native void nGetGlobalMatrix(long ref, float[] m);
    private static native void nGetLocalMatrix(long ref, float[] m);
    private static native void nSetLocalMatrix(long ref, float[] m);
    private static native void nUpdateTransformation(long ref);

    public void setStatic(boolean _static) {
        nSetStatic(getRef(), _static);
    }

    public boolean isStatic() {
        return nIsStatic(getRef());
    }

    public boolean attach(csSpatialState child) {
        return nAttach(getRef(), child != null ? child.getRef() : 0);
    }

    public boolean detachSelf() {
        return nDetachSelf(getRef());
    }

    public boolean detach(csSpatialState child) {
        return nDetach(getRef(), child != null ? child.getRef() : 0);
    }

    public csSpatialState getParent() {
        return nGetParent(getRef());
    }

    public int getNumberOfChildren() {
        return nGetNumberOfChildren(getRef());
    }

    public csSpatialState getChild(int idx) {
        return nGetChild(getRef(), idx);
    }


    private final Matrix4f _globalMatrix = new Matrix4f();
    private final float[] _globalMatrixArray = new float[16];
    public Matrix4f getGlobalMatrix () {
        nGetGlobalMatrix(getRef(), _globalMatrixArray);
        _globalMatrix.set(_globalMatrixArray);
        return _globalMatrix;
    }


    private final Matrix4f _localMatrix = new Matrix4f();
    private final float[] _localMatrixArray = new float[16];
    public Matrix4f getLocalMatrix () {
        nGetLocalMatrix(getRef(), _localMatrixArray);
        _localMatrix.set(_localMatrixArray);
        return _localMatrix;
    }

    public void setLocalMatrix(Matrix4f matrix) {
        matrix.toArray(_localMatrixArray);
        nSetLocalMatrix(getRef(), _localMatrixArray);
    }

    public void updateTransformation () {
        nUpdateTransformation(getRef());
    }

    private final csTransform _transform = new csTransform(this);
    public csTransform getTransform () {
        csSpatialState parent = getParent();
        if (parent != null) {
            _transform.initialize(getLocalMatrix(), parent.getGlobalMatrix());
        }
        else {
            _transform.initialize(getLocalMatrix(), null);
        }
        return _transform;
    }
}
