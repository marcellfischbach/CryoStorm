package org.cryo.core.entity;

import org.cryo.core.math.Matrix3f;
import org.cryo.core.math.Matrix4f;
import org.cryo.core.math.Quaternion;
import org.cryo.core.math.Vector3f;

public class Transform {

    private final SpatialState spatialState;
    private final Vector3f translation = new Vector3f();
    private final Quaternion rotation = new Quaternion();
    private final Vector3f scale = new Vector3f(1, 1, 1);

    private final Matrix4f parentMatrix = new Matrix4f();
    private boolean dirty;
    private final Matrix4f matrix = new Matrix4f();
    private final Matrix4f globalMatrix = new Matrix4f();


    public Transform(SpatialState spatialState) {
        this.spatialState = spatialState;
    }


    public Transform set(Matrix4f matrix,
                         Matrix4f parentMatrix) {
        this.matrix.set(matrix);
        this.parentMatrix.set(parentMatrix);

        extractTRS();
        return this;
    }

    private void extractTRS() {
        matrix.getTranslation(this.translation);
        scale.set(matrix.getXAxis(new Vector3f()).length(),
                  matrix.getYAxis(new Vector3f()).length(),
                  matrix.getZAxis(new Vector3f()).length());

        Matrix3f rotMat = new Matrix3f(matrix.m00 / scale.x, matrix.m01 / scale.x, matrix.m02 / scale.x,
                                       matrix.m10 / scale.y, matrix.m11 / scale.y, matrix.m12 / scale.y,
                                       matrix.m20 / scale.z, matrix.m21 / scale.z, matrix.m22 / scale.z);
        rotation.set(Quaternion.fromMatrix(rotMat));
    }

    public Transform setTranslation(Vector3f translation) {
        this.translation.set(translation);
        dirty = true;
        return this;
    }

    public Transform setTranslation(float x, float y, float z) {
        translation.set(x, y, z);
        dirty = true;
        return this;
    }

    public Vector3f getTranslation(Vector3f out) {
        out.set(translation);
        return out;
    }


    public Transform setRotation(Quaternion rotation) {
        this.rotation.set(rotation);
        dirty = true;
        return this;
    }


    public Quaternion getTranslation(Quaternion out) {
        out.set(rotation);
        return out;
    }

    public Transform setScale(Vector3f scale) {
        this.scale.set(scale);
        dirty = true;
        return this;
    }

    public Transform setScale(float x, float y, float z) {
        this.scale.set(x, y, z);
        dirty = true;
        return this;
    }

    public Vector3f getScale(Vector3f out) {
        out.set(scale);
        return out;
    }

    public Transform lookAt(Vector3f at, Vector3f up) {

        Vector3f from = this.parentMatrix.transform(this.translation, new Vector3f());


        Matrix4f tmpMatrix = new Matrix4f();
        tmpMatrix.setLookAtInv(from, at, up);

        Matrix4f parent = new Matrix4f(parentMatrix);
        parent.invert();

        tmpMatrix.mul(parent, this.matrix);

        extractTRS();
        dirty = true;
        return this;
    }


    public Matrix4f getMatrix() {
        updateMatrix();
        return matrix;
    }


    public Matrix4f getGlobalMatrix() {
        updateMatrix();
        return globalMatrix;
    }

    public Vector3f getForward() {
        updateMatrix();
        return globalMatrix.mul(Vector3f.Z, new Vector3f());
    }

    public Vector3f getBackward() {
        updateMatrix();
        return globalMatrix.mul(Vector3f.NEG_Z, new Vector3f());
    }

    public Vector3f getLeft() {
        updateMatrix();
        return globalMatrix.mul(Vector3f.NEG_X, new Vector3f());
    }

    public Vector3f getRight() {
        updateMatrix();
        return globalMatrix.mul(Vector3f.X, new Vector3f());
    }


    public Vector3f getUp() {
        updateMatrix();
        return globalMatrix.mul(Vector3f.Y, new Vector3f());
    }

    public Vector3f getDown() {
        updateMatrix();
        return globalMatrix.mul(Vector3f.NEG_Y, new Vector3f());
    }


    public void finish() {
        updateMatrix();
        this.spatialState.setLocalMatrix(this.matrix);
    }


    void updateMatrix() {
        if (dirty) {
            this.rotation.toMatrix4(this.matrix);
            this.matrix.setTranslation(this.translation);
            this.matrix.scale(this.scale);

            this.globalMatrix.set(parentMatrix).mul(this.matrix, globalMatrix);
            this.dirty = false;
        }
    }


}
