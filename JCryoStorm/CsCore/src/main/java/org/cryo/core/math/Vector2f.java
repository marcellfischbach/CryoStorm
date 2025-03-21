package org.cryo.core.math;

public class Vector2f {

    public float x;
    public float y;

    public Vector2f() {
        this(0.0f, 0.0f);
    }

    public Vector2f(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public Vector2f(Vector2f vector) {
        this.x = vector.x;
        this.y = vector.y;
    }

    public Vector2f set(float x, float y) {
        this.x = x;
        this.y = y;
        return this;
    }

    public Vector2f set(Vector2f vector) {
        this.x = vector.x;
        this.y = vector.y;
        return this;
    }

    public Vector2f set(float[] mem) {
        this.x = mem[0];
        this.y = mem[1];
        return this;
    }

    public void get(float[] mem) {
        mem[0] = x;
        mem[1] = y;
    }

    public Vector2f add(Vector2f vector) {
        x += vector.x;
        y += vector.y;
        return this;
    }

    public Vector2f sub(Vector2f vector) {
        x -= vector.x;
        y -= vector.y;
        return this;
    }

    public Vector2f mul(Vector2f vector) {
        x *= vector.x;
        y *= vector.y;
        return this;
    }

    public Vector2f mul(float f) {
        x *= f;
        y *= f;
        return this;
    }


    public Vector2f div(Vector2f vector) {
        x /= vector.x;
        y /= vector.y;
        return this;
    }

    public Vector2f div(float f) {
        x /= f;
        y /= f;
        return this;
    }

    public Vector2f mod(Vector2f vector) {
        x %= vector.x;
        y %= vector.y;
        return this;
    }

    public Vector2f mod(float f) {
        x %= f;
        y %= f;
        return this;
    }

    public Vector2f negate() {
        x = -x;
        y = -y;
        return this;
    }

    public float dot(Vector2f vector) {
        return x * vector.x + y * vector.y;
    }

    public float length() {
        return (float) Math.sqrt(x * x + y * y);
    }

    public Vector2f normalize() {
        return div(length());
    }

    @Override public String toString()
    {
        return "{" + x + ", " + y + '}';
    }

}
