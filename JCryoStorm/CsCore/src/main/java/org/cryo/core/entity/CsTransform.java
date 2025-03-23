package org.cryo.core.entity;

import org.cryo.core.math.CsMatrix3f;
import org.cryo.core.math.CsMatrix4f;
import org.cryo.core.math.CsQuaternion;
import org.cryo.core.math.CsVector3f;

public class CsTransform
{

    private final CsSpatialState spatialState;
    private final CsVector3f translation = new CsVector3f();
    private final CsQuaternion rotation = new CsQuaternion();
    private final CsVector3f scale = new CsVector3f(1, 1, 1);

    private final CsMatrix4f parentMatrix = new CsMatrix4f();
    private boolean dirty;
    private final CsMatrix4f matrix = new CsMatrix4f();
    private final CsMatrix4f globalMatrix = new CsMatrix4f();


    public CsTransform(CsSpatialState spatialState) {
        this.spatialState = spatialState;
    }


    public CsTransform set(CsMatrix4f matrix,
                           CsMatrix4f parentMatrix) {
        this.matrix.set(matrix);
        this.parentMatrix.set(parentMatrix);

        extractTRS();
        return this;
    }

    private void extractTRS() {
        matrix.getTranslation(this.translation);
        scale.set(matrix.getXAxis(new CsVector3f()).length(),
                  matrix.getYAxis(new CsVector3f()).length(),
                  matrix.getZAxis(new CsVector3f()).length());

        CsMatrix3f rotMat = new CsMatrix3f(matrix.m00 / scale.x, matrix.m01 / scale.x, matrix.m02 / scale.x,
                                       matrix.m10 / scale.y, matrix.m11 / scale.y, matrix.m12 / scale.y,
                                       matrix.m20 / scale.z, matrix.m21 / scale.z, matrix.m22 / scale.z);
        rotation.set(CsQuaternion.fromMatrix(rotMat));
    }

    public CsTransform setTranslation(CsVector3f translation) {
        this.translation.set(translation);
        dirty = true;
        return this;
    }

    public CsTransform setTranslation(float x, float y, float z) {
        translation.set(x, y, z);
        dirty = true;
        return this;
    }

    public CsVector3f getTranslation(CsVector3f out) {
        out.set(translation);
        return out;
    }


    public CsTransform setRotation(CsQuaternion rotation) {
        this.rotation.set(rotation);
        dirty = true;
        return this;
    }


    public CsQuaternion getTranslation(CsQuaternion out) {
        out.set(rotation);
        return out;
    }

    public CsTransform setScale(CsVector3f scale) {
        this.scale.set(scale);
        dirty = true;
        return this;
    }

    public CsTransform setScale(float x, float y, float z) {
        this.scale.set(x, y, z);
        dirty = true;
        return this;
    }

    public CsVector3f getScale(CsVector3f out) {
        out.set(scale);
        return out;
    }

    public CsTransform lookAt(CsVector3f at, CsVector3f up) {

        CsVector3f from = this.parentMatrix.transform(this.translation, new CsVector3f());


        CsMatrix4f tmpMatrix = new CsMatrix4f();
        tmpMatrix.setLookAtInv(from, at, up);

        CsMatrix4f parent = new CsMatrix4f(parentMatrix);
        parent.invert();

        tmpMatrix.mul(parent, this.matrix);

        extractTRS();
        dirty = true;
        return this;
    }


    public CsMatrix4f getMatrix() {
        updateMatrix();
        return matrix;
    }


    public CsMatrix4f getGlobalMatrix() {
        updateMatrix();
        return globalMatrix;
    }

    public CsVector3f getForward() {
        updateMatrix();
        return globalMatrix.mul(CsVector3f.Z, new CsVector3f());
    }

    public CsVector3f getBackward() {
        updateMatrix();
        return globalMatrix.mul(CsVector3f.NEG_Z, new CsVector3f());
    }

    public CsVector3f getLeft() {
        updateMatrix();
        return globalMatrix.mul(CsVector3f.NEG_X, new CsVector3f());
    }

    public CsVector3f getRight() {
        updateMatrix();
        return globalMatrix.mul(CsVector3f.X, new CsVector3f());
    }


    public CsVector3f getUp() {
        updateMatrix();
        return globalMatrix.mul(CsVector3f.Y, new CsVector3f());
    }

    public CsVector3f getDown() {
        updateMatrix();
        return globalMatrix.mul(CsVector3f.NEG_Y, new CsVector3f());
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
