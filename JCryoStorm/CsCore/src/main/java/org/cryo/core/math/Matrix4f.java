package org.cryo.core.math;

public class Matrix4f {

    public static final Matrix4f IDENTITY = new Matrix4f();

    public float m00;
    public float m01;
    public float m02;
    public float m03;

    public float m10;
    public float m11;
    public float m12;
    public float m13;

    public float m20;
    public float m21;
    public float m22;
    public float m23;

    public float m30;
    public float m31;
    public float m32;
    public float m33;

    public Matrix4f() {
        this(1.0f, 0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 1.0f, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f);
    }

    public Matrix4f(float m00, float m01, float m02, float m03,
                    float m10, float m11, float m12, float m13,
                    float m20, float m21, float m22, float m23,
                    float m30, float m31, float m32, float m33
                   ) {
        this.m00 = m00;
        this.m01 = m01;
        this.m02 = m02;
        this.m03 = m03;
        this.m10 = m10;
        this.m11 = m11;
        this.m12 = m12;
        this.m13 = m13;
        this.m20 = m20;
        this.m21 = m21;
        this.m22 = m22;
        this.m23 = m23;
        this.m30 = m30;
        this.m31 = m31;
        this.m32 = m32;
        this.m33 = m33;
    }

    public Matrix4f(Matrix4f m) {
        this.m00 = m.m00;
        this.m01 = m.m01;
        this.m02 = m.m02;
        this.m03 = m.m03;
        this.m10 = m.m10;
        this.m11 = m.m11;
        this.m12 = m.m12;
        this.m13 = m.m13;
        this.m20 = m.m20;
        this.m21 = m.m21;
        this.m22 = m.m22;
        this.m23 = m.m23;
        this.m30 = m.m30;
        this.m31 = m.m31;
        this.m32 = m.m32;
        this.m33 = m.m33;
    }

    public Matrix4f set(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33
                       ) {
        this.m00 = m00;
        this.m01 = m01;
        this.m02 = m02;
        this.m03 = m03;
        this.m10 = m10;
        this.m11 = m11;
        this.m12 = m12;
        this.m13 = m13;
        this.m20 = m20;
        this.m21 = m21;
        this.m22 = m22;
        this.m23 = m23;
        this.m30 = m30;
        this.m31 = m31;
        this.m32 = m32;
        this.m33 = m33;
        return this;
    }


    public Matrix4f set(Matrix4f m) {
        this.m00 = m.m00;
        this.m01 = m.m01;
        this.m02 = m.m02;
        this.m03 = m.m03;
        this.m10 = m.m10;
        this.m11 = m.m11;
        this.m12 = m.m12;
        this.m13 = m.m13;
        this.m20 = m.m20;
        this.m21 = m.m21;
        this.m22 = m.m22;
        this.m23 = m.m23;
        this.m30 = m.m30;
        this.m31 = m.m31;
        this.m32 = m.m32;
        this.m33 = m.m33;
        return this;
    }


    public Matrix4f set(float[] f) {
        m00 = f[0];
        m01 = f[1];
        m02 = f[2];
        m03 = f[3];
        m10 = f[4];
        m11 = f[5];
        m12 = f[6];
        m13 = f[7];
        m20 = f[8];
        m21 = f[9];
        m22 = f[10];
        m23 = f[11];
        m30 = f[12];
        m31 = f[13];
        m32 = f[14];
        m33 = f[15];
        return this;
    }

    public void get(float[] f) {
        f[0] = m00;
        f[1] = m01;
        f[2] = m02;
        f[3] = m03;
        f[4] = m10;
        f[5] = m11;
        f[6] = m12;
        f[7] = m13;
        f[8] = m20;
        f[9] = m21;
        f[10] = m22;
        f[11] = m23;
        f[12] = m30;
        f[13] = m31;
        f[14] = m32;
        f[15] = m33;
    }

    public void setXAxis(Vector3f xAxis) {
        this.m00 = xAxis.x;
        this.m01 = xAxis.y;
        this.m02 = xAxis.z;
        this.m03 = 0.0f;
    }

    public void setXAxis(float x, float y, float z) {
        this.m00 = x;
        this.m01 = y;
        this.m02 = z;
        this.m03 = 0.0f;
    }

    public Vector3f getXAxis(Vector3f out) {
        return out.set(m00, m01, m02);
    }

    public void setYAxis(Vector3f yAxis) {
        this.m10 = yAxis.x;
        this.m11 = yAxis.y;
        this.m12 = yAxis.z;
        this.m13 = 0.0f;
    }

    public void setYAxis(float x, float y, float z) {
        this.m10 = x;
        this.m11 = y;
        this.m12 = z;
        this.m13 = 0.0f;
    }

    public Vector3f getYAxis(Vector3f out) {
        return out.set(m10, m11, m12);
    }

    public void setZAxis(Vector3f zAxis) {
        this.m20 = zAxis.x;
        this.m21 = zAxis.y;
        this.m22 = zAxis.z;
        this.m23 = 0.0f;
    }

    public void setZAxis(float x, float y, float z) {
        this.m20 = x;
        this.m21 = y;
        this.m22 = z;
        this.m23 = 0.0f;
    }

    public Vector3f getZAxis(Vector3f out) {
        return out.set(m20, m21, m22);
    }

    public void setTranslation(Vector3f translation) {
        this.m30 = translation.x;
        this.m31 = translation.y;
        this.m32 = translation.z;
        this.m33 = 1.0f;
    }

    public void setTranslation(float x, float y, float z) {
        this.m30 = x;
        this.m31 = y;
        this.m32 = z;
        this.m33 = 1.0f;
    }

    public Vector3f getTranslation(Vector3f out) {
        return out.set(m30, m31, m32);
    }


    public void clearRotation() {
        m00 = m11 = m22 = 1.0f;
        m01 = m02 = m10 = m12 = m20 = m21 = 0.0f;
    }


    public void setRotationX(float angle) {
        float c = (float) Math.cos(angle);
        float s = (float) Math.sin(angle);
        m11 = c;
        m12 = s;
        m21 = -s;
        m22 = c;
    }

    public void setRotationY(float angle) {
        float c = (float) Math.cos(angle);
        float s = (float) Math.sin(angle);
        m00 = c;
        m02 = -s;
        m20 = s;
        m22 = c;
    }

    public void setRotationZ(float angle) {
        float c = (float) Math.cos(angle);
        float s = (float) Math.sin(angle);
        m00 = c;
        m01 = s;
        m10 = -s;
        m11 = c;
    }

    public void setScale(float x, float y, float z) {
        m00 = x;
        m11 = y;
        m22 = z;
    }

    public void setScale(Vector3f s) {
        m00 = s.x;
        m11 = s.y;
        m22 = s.z;
    }

    public void scale(Vector3f s) {
        m00 *= s.x;
        m11 *= s.y;
        m22 *= s.z;
    }

    public Matrix4f setRotation(Vector3f axis, float angle) {
        float c = (float) Math.cos(angle);
        float s = (float) Math.sin(angle);

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        float ic = 1.0f - c;
        m00 = x * x * ic + c;
        m10 = x * y * ic - z * s;
        m20 = x * z * ic + y * s;
        m01 = y * x * ic + z * s;
        m11 = y * y * ic + c;
        m21 = y * z * ic - x * s;
        m02 = z * x * ic - y * s;
        m12 = z * y * ic + x * s;
        m22 = z * z * ic + c;

        return this;
    }

    public Matrix4f mul(Matrix4f m, Matrix4f out) {
        float mm00 = this.m00 * m.m00 + this.m10 * m.m01 + this.m20 * m.m02 + this.m30 * m.m03;
        float mm01 = this.m01 * m.m00 + this.m11 * m.m01 + this.m21 * m.m02 + this.m31 * m.m03;
        float mm02 = this.m02 * m.m00 + this.m12 * m.m01 + this.m22 * m.m02 + this.m32 * m.m03;
        float mm03 = this.m03 * m.m00 + this.m13 * m.m01 + this.m23 * m.m02 + this.m33 * m.m03;

        float mm10 = this.m00 * m.m10 + this.m10 * m.m11 + this.m20 * m.m12 + this.m30 * m.m13;
        float mm11 = this.m01 * m.m10 + this.m11 * m.m11 + this.m21 * m.m12 + this.m31 * m.m13;
        float mm12 = this.m02 * m.m10 + this.m12 * m.m11 + this.m22 * m.m12 + this.m32 * m.m13;
        float mm13 = this.m03 * m.m10 + this.m13 * m.m11 + this.m23 * m.m12 + this.m33 * m.m13;

        float mm20 = this.m00 * m.m20 + this.m10 * m.m21 + this.m20 * m.m22 + this.m30 * m.m23;
        float mm21 = this.m01 * m.m20 + this.m11 * m.m21 + this.m21 * m.m22 + this.m31 * m.m23;
        float mm22 = this.m02 * m.m20 + this.m12 * m.m21 + this.m22 * m.m22 + this.m32 * m.m23;
        float mm23 = this.m03 * m.m20 + this.m13 * m.m21 + this.m23 * m.m22 + this.m33 * m.m23;

        float mm30 = this.m00 * m.m30 + this.m10 * m.m31 + this.m20 * m.m32 + this.m30 * m.m33;
        float mm31 = this.m01 * m.m30 + this.m11 * m.m31 + this.m21 * m.m32 + this.m31 * m.m33;
        float mm32 = this.m02 * m.m30 + this.m12 * m.m31 + this.m22 * m.m32 + this.m32 * m.m33;
        float mm33 = this.m03 * m.m30 + this.m13 * m.m31 + this.m23 * m.m32 + this.m33 * m.m33;

        return out.set(mm00, mm01, mm02, mm03,
                       mm10, mm11, mm12, mm13,
                       mm20, mm21, mm22, mm23,
                       mm30, mm31, mm32, mm33
                      );
    }

    public Vector3f mul(Vector3f v, Vector3f out) {
        float x = m00 * v.x + m10 * v.y + m20 * v.z;
        float y = m01 * v.x + m11 * v.y + m21 * v.z;
        float z = m02 * v.x + m12 * v.y + m22 * v.z;
        return out.set(x, y, z);
    }


    public Vector3f transform(Vector3f v, Vector3f out) {
        float x = m00 * v.x + m10 * v.y + m20 * v.z + m30;
        float y = m01 * v.x + m11 * v.y + m21 * v.z + m31;
        float z = m02 * v.x + m12 * v.y + m22 * v.z + m32;
        return out.set(x, y, z);
    }

    public Matrix4f setLookAt(Vector3f eye, Vector3f spot, Vector3f up) {

        Vector3f zAxis = new Vector3f(spot).sub(eye).normalize();
        Vector3f xAxis = new Vector3f(up).cross(zAxis).normalize();
        Vector3f yAxis = new Vector3f(zAxis).cross(xAxis).normalize();

        setXAxis(xAxis.x, yAxis.x, zAxis.x);
        setYAxis(xAxis.y, yAxis.y, zAxis.y);
        setZAxis(xAxis.z, yAxis.z, zAxis.z);
        setTranslation(-xAxis.dot(eye), -yAxis.dot(eye), -zAxis.dot(eye));

        return this;
    }

    public Matrix4f setLookAtInv(Vector3f eye, Vector3f spot, Vector3f up) {
        Vector3f zAxis = new Vector3f(spot).sub(eye).normalize();
        Vector3f xAxis = new Vector3f(up).cross(zAxis).normalize();
        Vector3f yAxis = new Vector3f(zAxis).cross(xAxis);

        setXAxis(xAxis);
        setYAxis(yAxis);
        setZAxis(zAxis);
        setTranslation(eye);
        return this;
    }


    public Matrix4f Inverted() {
        float v0 = m20 * m31 - m21 * m30;
        float v1 = m20 * m32 - m22 * m30;
        float v2 = m20 * m33 - m23 * m30;
        float v3 = m21 * m32 - m22 * m31;
        float v4 = m21 * m33 - m23 * m31;
        float v5 = m22 * m33 - m23 * m32;

        float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
        float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
        float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
        float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

        float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        float lm00 = t00 * invDet;
        float lm10 = t10 * invDet;
        float lm20 = t20 * invDet;
        float lm30 = t30 * invDet;

        float lm01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float lm11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float lm21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float lm31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        float lm02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float lm12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float lm22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float lm32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        float lm03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float lm13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float lm23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float lm33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        return new Matrix4f(lm00, lm01, lm02, lm03,
                            lm10, lm11, lm12, lm13,
                            lm20, lm21, lm22, lm23,
                            lm30, lm31, lm32, lm33);
    }

    public Matrix4f invert() {
        float v0 = m20 * m31 - m21 * m30;
        float v1 = m20 * m32 - m22 * m30;
        float v2 = m20 * m33 - m23 * m30;
        float v3 = m21 * m32 - m22 * m31;
        float v4 = m21 * m33 - m23 * m31;
        float v5 = m22 * m33 - m23 * m32;

        float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
        float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
        float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
        float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

        float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        float lm00 = t00 * invDet;
        float lm10 = t10 * invDet;
        float lm20 = t20 * invDet;
        float lm30 = t30 * invDet;

        float lm01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float lm11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float lm21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float lm31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        float lm02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float lm12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float lm22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float lm32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        float lm03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float lm13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float lm23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float lm33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        m00 = lm00;
        m01 = lm01;
        m02 = lm02;
        m03 = lm03;

        m10 = lm10;
        m11 = lm11;
        m12 = lm12;
        m13 = lm13;

        m20 = lm20;
        m21 = lm21;
        m22 = lm22;
        m23 = lm23;

        m30 = lm30;
        m31 = lm31;
        m32 = lm32;
        m33 = lm33;
        return this;
    }
}
