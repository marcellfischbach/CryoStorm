package org.cryo.core.math;

public class Matrix3f {

    public float m00;
    public float m01;
    public float m02;

    public float m10;
    public float m11;
    public float m12;

    public float m20;
    public float m21;
    public float m22;


    public Matrix3f set (float[] f) {
        m00 = f[0];
        m01 = f[1];
        m02 = f[2];
        m10 = f[3];
        m11 = f[4];
        m12 = f[5];
        m20 = f[6];
        m21 = f[7];
        m22 = f[8];
        return this;
    }

    public void get(float[] f) {
        f[0] = m00;
        f[1] = m01;
        f[2] = m02;
        f[3] = m10;
        f[4] = m11;
        f[5] = m12;
        f[6] = m20;
        f[7] = m21;
        f[8] = m22;
    }

}
