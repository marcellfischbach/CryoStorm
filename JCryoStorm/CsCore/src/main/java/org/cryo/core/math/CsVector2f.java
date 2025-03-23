package org.cryo.core.math;

public class CsVector2f
{

    public float x;
    public float y;

    public CsVector2f() {
        this(0.0f, 0.0f);
    }

    public CsVector2f(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public CsVector2f(CsVector2f vector) {
        this.x = vector.x;
        this.y = vector.y;
    }

    public CsVector2f set(float x, float y) {
        this.x = x;
        this.y = y;
        return this;
    }

    public CsVector2f set(CsVector2f vector) {
        this.x = vector.x;
        this.y = vector.y;
        return this;
    }

    public CsVector2f set(float[] mem) {
        this.x = mem[0];
        this.y = mem[1];
        return this;
    }

    public void get(float[] mem) {
        mem[0] = x;
        mem[1] = y;
    }

    public CsVector2f add(CsVector2f vector) {
        x += vector.x;
        y += vector.y;
        return this;
    }

    public CsVector2f sub(CsVector2f vector) {
        x -= vector.x;
        y -= vector.y;
        return this;
    }

    public CsVector2f mul(CsVector2f vector) {
        x *= vector.x;
        y *= vector.y;
        return this;
    }

    public CsVector2f mul(float f) {
        x *= f;
        y *= f;
        return this;
    }


    public CsVector2f div(CsVector2f vector) {
        x /= vector.x;
        y /= vector.y;
        return this;
    }

    public CsVector2f div(float f) {
        x /= f;
        y /= f;
        return this;
    }

    public CsVector2f mod(CsVector2f vector) {
        x %= vector.x;
        y %= vector.y;
        return this;
    }

    public CsVector2f mod(float f) {
        x %= f;
        y %= f;
        return this;
    }

    public CsVector2f negate() {
        x = -x;
        y = -y;
        return this;
    }

    public float dot(CsVector2f vector) {
        return x * vector.x + y * vector.y;
    }

    public float length() {
        return (float) Math.sqrt(x * x + y * y);
    }

    public CsVector2f normalize() {
        return div(length());
    }

    @Override public String toString()
    {
        return "{" + x + ", " + y + '}';
    }

}
