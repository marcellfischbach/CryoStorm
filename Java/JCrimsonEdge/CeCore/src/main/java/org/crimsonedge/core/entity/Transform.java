package org.crimsonedge.core.entity;

import org.crimsonedge.core.math.Matrix3f;
import org.crimsonedge.core.math.Matrix4f;
import org.crimsonedge.core.math.Quaternion;
import org.crimsonedge.core.math.Vector3f;

public class csTransform {

    private final csSpatialState spatialState;

    private final Vector3f translation = new Vector3f();

    private final Quaternion rotation = new Quaternion();

    private final Vector3f scale = new Vector3f();

    private final Matrix4f parentMatrix = new Matrix4f();

    private boolean dirty;

    private final Matrix4f matrix = new Matrix4f();

    private final Matrix4f globalMatrix = new Matrix4f();

    public csTransform(csSpatialState spatialState) {
        this.spatialState = spatialState;
    }

    public void initialize(Matrix4f local, Matrix4f parent) {
        this.matrix.set(local);
        this.translation.set(0.0f, 0.0f, 0.0f);
        this.rotation.set(0.0f, 0.0f, 0.0f, 1.0f);
        this.scale.set(1.0f, 1.0f, 1.0f);
        this.dirty = true;
        if (parent != null) {
            parentMatrix.set(parent);
        } else {
            parentMatrix.setIdentity();
        }

        extractTRS();
    }

    private final Vector3f tmpV3 = new Vector3f();
    private final Matrix3f tmpM3 = new Matrix3f();

    private void extractTRS() {
        matrix.getTranslation(this.translation);
        scale.set(
                matrix.getXAxis(tmpV3).length(),
                matrix.getYAxis(tmpV3).length(),
                matrix.getZAxis(tmpV3).length()
        );
        tmpM3.set(
                matrix.m00 / scale.x, matrix.m01 / scale.x, matrix.m02 / scale.x,
                matrix.m10 / scale.y, matrix.m11 / scale.y, matrix.m12 / scale.y,
                matrix.m20 / scale.z, matrix.m21 / scale.z, matrix.m22 / scale.z
        );
        rotation.from(tmpM3);
    }


    public csTransform setTranslation(Vector3f translation) {
        this.translation.set(translation);
        this.dirty = true;
        return this;
    }

    public csTransform setTranslation(float x, float y, float z) {
        this.translation.set(x, y, z);
        this.dirty = true;
        return this;
    }

    public Vector3f getTranslation(Vector3f t) {
        t.set(this.translation);
        return t;
    }

    public csTransform setRotation(Quaternion rotation) {
        this.rotation.set(rotation);
        this.dirty = true;
        return this;
    }

    public Quaternion getRotation(Quaternion r) {
        r.set(this.rotation);
        return r;
    }

    public csTransform setScale(Vector3f scale) {
        this.scale.set(scale);
        this.dirty = true;
        return this;
    }

    public Vector3f getScale(Vector3f s) {
        s.set(this.scale);
        return s;
    }

    public Matrix4f getMatrix() {
        updateMatrix();
        return this.matrix;
    }


    public Matrix4f getMatrix(Matrix4f m) {
        updateMatrix();
        m.set(this.matrix);
        return m;
    }

    public Matrix4f getGlobalMatrix() {
        updateMatrix();
        return this.globalMatrix;
    }

    public Matrix4f getGlobalMatrix(Matrix4f m) {
        updateMatrix();
        m.set(this.globalMatrix);
        return m;
    }

    public Vector3f getForward(Vector3f result) {
        updateMatrix();
        globalMatrix.mult(Vector3f.FORWARD, result);
        return result;
    }

    public Vector3f getBackward(Vector3f result) {
        updateMatrix();
        globalMatrix.mult(Vector3f.BACKWARD, result);
        return result;
    }

    public Vector3f getLeft(Vector3f result) {
        updateMatrix();
        globalMatrix.mult(Vector3f.LEFT, result);
        return result;
    }

    public Vector3f getRight(Vector3f result) {
        updateMatrix();
        globalMatrix.mult(Vector3f.RIGHT, result);
        return result;
    }

    public Vector3f getUp(Vector3f result) {
        updateMatrix();
        globalMatrix.mult(Vector3f.UP, result);
        return result;
    }

    public Vector3f getDown(Vector3f result) {
        updateMatrix();
        globalMatrix.mult(Vector3f.DOWN, result);
        return result;
    }

    public void finish() {
        updateMatrix();
        this.spatialState.setLocalMatrix(matrix);
    }

    private void updateMatrix() {
        if (dirty) {
            this.rotation.toMatrix(this.matrix);
            this.matrix.setTranslation(this.translation);
            this.matrix.scale(this.scale);
            Matrix4f.mult(this.parentMatrix, this.matrix, this.globalMatrix);
            dirty = false;
        }
    }
}
