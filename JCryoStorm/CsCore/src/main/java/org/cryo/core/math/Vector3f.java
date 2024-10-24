package org.cryo.core.math;

public class Vector3f {

    public float x;
    public float y;
    public float z;

    public Vector3f() {
        this(0.0f, 0.0f, 0.0f);
    }
    public Vector3f(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public Vector3f(Vector3f vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
    }

    public Vector3f set(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
        return this;
    }

    public Vector3f set(Vector3f vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
        return this;
    }

    public Vector3f set (float[] mem) {
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

    public Vector3f add (Vector3f vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return this;
    }

    public Vector3f sub (Vector3f vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return this;
    }

    public Vector3f mul (Vector3f vector) {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        return this;
    }

    public Vector3f mul (float f) {
        x *= f;
        y *= f;
        z *= f;
        return this;
    }


    public Vector3f div (Vector3f vector) {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        return this;
    }

    public Vector3f div (float f) {
        x /= f;
        y /= f;
        z /= f;
        return this;
    }

    public Vector3f mod (Vector3f vector) {
        x %= vector.x;
        y %= vector.y;
        z %= vector.z;
        return this;
    }

    public Vector3f mod (float f) {
        x %= f;
        y %= f;
        z %= f;
        return this;
    }

    public Vector3f negate () {
        x = -x;
        y = -y;
        z = -z;
        return this;
    }

    public Vector3f cross (Vector3f vector) {
        float x = y * vector.z - z * vector.y;
        float y = z * vector.x - x * vector.z;
        float z = x * vector.y - y * vector.x;
        return set(x, y, z);
    }

    public float dot (Vector3f vector) {
        return x * vector.x + y * vector.y + z * vector.z;
    }

    public float length () {
        return (float) Math.sqrt(x * x + y * y + z * z);
    }

    public Vector3f normalize() {
        return div(length());
    }
}
