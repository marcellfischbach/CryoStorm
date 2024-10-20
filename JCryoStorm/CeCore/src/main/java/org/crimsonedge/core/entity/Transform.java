package org.crimsonedge.core.entity;

import org.crimsonedge.core.math.csMatrix3f;
import org.crimsonedge.core.math.csMatrix4f;
import org.crimsonedge.core.math.csQuaternion;
import org.crimsonedge.core.math.csVector3f;

public class csTransform {

    private final csSpatialState spatialState;

    private final csVector3f translation = new csVector3f();

    private final csQuaternion rotation = new csQuaternion();

    private final csVector3f scale = new csVector3f();

    private final csMatrix4f parentMatrix = new csMatrix4f();

    private boolean dirty;

    private final csMatrix4f matrix = new csMatrix4f();

    private final csMatrix4f globalMatrix = new csMatrix4f();

    public csTransform(csSpatialState spatialState) {
        this.spatialState = spatialState;
    }

    public void initialize(csMatrix4f local, csMatrix4f parent) {
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

    private final csVector3f tmpV3 = new csVector3f();
    private final csMatrix3f tmpM3 = new csMatrix3f();

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


    public csTransform setTranslation(csVector3f translation) {
        this.translation.set(translation);
        this.dirty = true;
        return this;
    }

    public csTransform setTranslation(float x, float y, float z) {
        this.translation.set(x, y, z);
        this.dirty = true;
        return this;
    }

    public csVector3f getTranslation(csVector3f t) {
        t.set(this.translation);
        return t;
    }

    public csTransform setRotation(csQuaternion rotation) {
        this.rotation.set(rotation);
        this.dirty = true;
        return this;
    }

    public csQuaternion getRotation(csQuaternion r) {
        r.set(this.rotation);
        return r;
    }

    public csTransform setScale(csVector3f scale) {
        this.scale.set(scale);
        this.dirty = true;
        return this;
    }

    public csVector3f getScale(csVector3f s) {
        s.set(this.scale);
        return s;
    }

    public csMatrix4f getMatrix() {
        updateMatrix();
        return this.matrix;
    }


    public csMatrix4f getMatrix(csMatrix4f m) {
        updateMatrix();
        m.set(this.matrix);
        return m;
    }

    public csMatrix4f getGlobalMatrix() {
        updateMatrix();
        return this.globalMatrix;
    }

    public csMatrix4f getGlobalMatrix(csMatrix4f m) {
        updateMatrix();
        m.set(this.globalMatrix);
        return m;
    }

    public csVector3f getForward(csVector3f result) {
        updateMatrix();
        globalMatrix.mult(csVector3f.FORWARD, result);
        return result;
    }

    public csVector3f getBackward(csVector3f result) {
        updateMatrix();
        globalMatrix.mult(csVector3f.BACKWARD, result);
        return result;
    }

    public csVector3f getLeft(csVector3f result) {
        updateMatrix();
        globalMatrix.mult(csVector3f.LEFT, result);
        return result;
    }

    public csVector3f getRight(csVector3f result) {
        updateMatrix();
        globalMatrix.mult(csVector3f.RIGHT, result);
        return result;
    }

    public csVector3f getUp(csVector3f result) {
        updateMatrix();
        globalMatrix.mult(csVector3f.UP, result);
        return result;
    }

    public csVector3f getDown(csVector3f result) {
        updateMatrix();
        globalMatrix.mult(csVector3f.DOWN, result);
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
            csMatrix4f.mult(this.parentMatrix, this.matrix, this.globalMatrix);
            dirty = false;
        }
    }
}
