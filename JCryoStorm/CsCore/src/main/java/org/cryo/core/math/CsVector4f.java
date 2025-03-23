package org.cryo.core.math;

public class CsVector4f
{

    public float x;
    public float y;
    public float z;
    public float w;

    public CsVector4f() {
        this(0.0f, 0.0f, 0.0f, 1.0f);
    }
    public CsVector4f(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    public CsVector4f(CsVector4f vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
        this.w = vector.w;
    }

    public CsVector4f set(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
        return this;
    }

    public CsVector4f set(CsVector4f vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
        this.w = vector.w;
        return this;
    }

    public CsVector4f set (float[] mem) {
        this.x = mem[0];
        this.y = mem[1];
        this.z = mem[2];
        this.w = mem[3];
        return this;
    }

    public void get (float[] mem) {
        mem[0] = x;
        mem[1] = y;
        mem[2] = z;
        mem[3] = w;
    }

    public CsVector4f add (CsVector4f vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        w += vector.w;
        return this;
    }

    public CsVector4f sub (CsVector4f vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        w -= vector.w;
        return this;
    }

    public CsVector4f mul (CsVector4f vector) {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        w *= vector.w;
        return this;
    }

    public CsVector4f mul (float f) {
        x *= f;
        y *= f;
        z *= f;
        w *= f;
        return this;
    }


    public CsVector4f div (CsVector4f vector) {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        w /= vector.w;
        return this;
    }

    public CsVector4f div (float f) {
        x /= f;
        y /= f;
        z /= f;
        w /= f;
        return this;
    }

    public CsVector4f mod (CsVector4f vector) {
        x %= vector.x;
        y %= vector.y;
        z %= vector.z;
        w %= vector.w;
        return this;
    }

    public CsVector4f mod (float f) {
        x %= f;
        y %= f;
        z %= f;
        w %= f;
        return this;
    }

    public CsVector4f negate () {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
        return this;
    }


    public float dot (CsVector4f vector) {
        return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
    }

    public float length () {
        return (float) Math.sqrt(x * x + y * y + z * z + w * w);
    }

    public CsVector4f normalize() {
        return div(length());
    }

}
