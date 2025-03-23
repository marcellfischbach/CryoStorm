package org.cryo.core.math;

public class CsMatrix3f
{

    public float m00;
    public float m01;
    public float m02;

    public float m10;
    public float m11;
    public float m12;

    public float m20;
    public float m21;
    public float m22;

    public CsMatrix3f() {
        this(1.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             0.0f, 0.0f, 1.0f);
    }

    public CsMatrix3f(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
        this.m00 = m00;
        this.m01 = m01;
        this.m02 = m02;
        this.m10 = m10;
        this.m11 = m11;
        this.m12 = m12;
        this.m20 = m20;
        this.m21 = m21;
        this.m22 = m22;
    }

    public void set(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
        this.m00 = m00;
        this.m01 = m01;
        this.m02 = m02;
        this.m10 = m10;
        this.m11 = m11;
        this.m12 = m12;
        this.m20 = m20;
        this.m21 = m21;
        this.m22 = m22;
    }

    public CsMatrix3f set(float[] f) {
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
