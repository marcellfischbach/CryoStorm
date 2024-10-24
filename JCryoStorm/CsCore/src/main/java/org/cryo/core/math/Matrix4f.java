package org.cryo.core.math;

public class Matrix4f {

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

}
