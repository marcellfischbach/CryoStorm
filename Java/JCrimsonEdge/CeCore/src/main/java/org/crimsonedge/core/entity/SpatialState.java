package org.crimsonedge.core.entity;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.math.Matrix4f;

@CeClass("cryo::SpatialState")
public class SpatialState extends EntityState {

    public SpatialState() {
    }

    public SpatialState(long ref) {
        super(ref);
    }


    private static native void nSetStatic(long ref, boolean _static);

    private static native boolean nIsStatic(long ref);

    private static native boolean nAttach(long ref, long childRef);

    private static native boolean nDetachSelf(long ref);

    private static native boolean nDetach(long ref, long childRef);

    private static native SpatialState nGetParent(long ref);

    private static native int nGetNumberOfChildren(long ref);

    private static native SpatialState nGetChild(long ref, int idx);

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

    public boolean attach(SpatialState child) {
        return nAttach(getRef(), child != null ? child.getRef() : 0);
    }

    public boolean detachSelf() {
        return nDetachSelf(getRef());
    }

    public boolean detach(SpatialState child) {
        return nDetach(getRef(), child != null ? child.getRef() : 0);
    }

    public SpatialState getParent() {
        return nGetParent(getRef());
    }

    public int getNumberOfChildren() {
        return nGetNumberOfChildren(getRef());
    }

    public SpatialState getChild(int idx) {
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

    private final Transform _transform = new Transform(this);
    public Transform getTransform () {
        SpatialState parent = getParent();
        if (parent != null) {
            _transform.initialize(getLocalMatrix(), parent.getGlobalMatrix());
        }
        else {
            _transform.initialize(getLocalMatrix(), null);
        }
        return _transform;
    }
}
