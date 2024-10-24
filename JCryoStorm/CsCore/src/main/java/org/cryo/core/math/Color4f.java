package org.cryo.core.math;

public class Color4f {

    public float r;
    public float g;
    public float b;
    public float a;

    public Color4f() {
        this(0.0f, 0.0f, 0.0f, 1.0f);
    }

    public Color4f(float r, float g, float b, float a) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    public Color4f set (float[] mem) {
        this.r = mem[0];
        this.g = mem[1];
        this.b = mem[2];
        this.a = mem[3];
        return this;
    }

    public void get (float[] mem) {
        mem[0] = r;
        mem[1] = g;
        mem[2] = b;
        mem[3] = a;
    }

}
