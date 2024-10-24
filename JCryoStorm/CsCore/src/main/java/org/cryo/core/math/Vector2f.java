package org.cryo.core.math;

public class Vector2f {

    public float x;
    public float y;

    public Vector2f set (float[] mem) {
        this.x = mem[0];
        this.y = mem[1];
        return this;
    }

    public void get (float[] mem) {
        mem[0] = x;
        mem[1] = y;
    }
}
