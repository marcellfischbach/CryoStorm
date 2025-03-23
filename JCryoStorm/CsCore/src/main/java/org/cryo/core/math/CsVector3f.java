package org.cryo.core.math;

public class CsVector3f
{

    public float x;
    public float y;
    public float z;

    public static final CsVector3f ZERO = new CsVector3f(0, 0, 0);
    public static final CsVector3f ONE = new CsVector3f(1, 1, 1);
    public static final CsVector3f X = new CsVector3f(1, 0, 0);
    public static final CsVector3f Y = new CsVector3f(0, 1, 0);
    public static final CsVector3f Z = new CsVector3f(0, 0, 1);
    public static final CsVector3f NEG_X = new CsVector3f(-1, 0, 0);
    public static final CsVector3f NEG_Y = new CsVector3f(0, -1, 0);
    public static final CsVector3f NEG_Z = new CsVector3f(0, 0, -1);



    public CsVector3f() {
        this(0.0f, 0.0f, 0.0f);
    }
    public CsVector3f(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public CsVector3f(CsVector3f vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
    }

    public CsVector3f set(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
        return this;
    }

    public CsVector3f set(CsVector3f vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
        return this;
    }

    public CsVector3f set (float[] mem) {
        this.x = mem[0];
        this.y = mem[1];
        this.z = mem[2];
        return this;
    }

    public void get (float[] mem) {
        mem[0] = x;
        mem[1] = y;
        mem[2] = z;
    }

    public CsVector3f add (CsVector3f vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return this;
    }

    public CsVector3f sub (CsVector3f vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return this;
    }

    public CsVector3f mul (CsVector3f vector) {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        return this;
    }

    public CsVector3f mul (float f) {
        x *= f;
        y *= f;
        z *= f;
        return this;
    }


    public CsVector3f div (CsVector3f vector) {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        return this;
    }

    public CsVector3f div (float f) {
        x /= f;
        y /= f;
        z /= f;
        return this;
    }

    public CsVector3f mod (CsVector3f vector) {
        x %= vector.x;
        y %= vector.y;
        z %= vector.z;
        return this;
    }

    public CsVector3f mod (float f) {
        x %= f;
        y %= f;
        z %= f;
        return this;
    }

    public CsVector3f negate () {
        x = -x;
        y = -y;
        z = -z;
        return this;
    }

    public CsVector3f cross (CsVector3f vector) {
        float x = this.y * vector.z - this.z * vector.y;
        float y = this.z * vector.x - this.x * vector.z;
        float z = this.x * vector.y - this.y * vector.x;
        return set(x, y, z);
    }

    public float dot (CsVector3f vector) {
        return x * vector.x + y * vector.y + z * vector.z;
    }

    public float length () {
        return (float) Math.sqrt(x * x + y * y + z * z);
    }

    public CsVector3f normalize() {
        return div(length());
    }

    @Override
    public String toString() {
        return "{" +
                x +
                "," + y +
                ", " + z +
                '}';
    }
}
