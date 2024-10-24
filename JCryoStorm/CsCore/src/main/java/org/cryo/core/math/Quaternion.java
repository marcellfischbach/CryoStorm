package org.cryo.core.math;

public class Quaternion {

    float x;
    float y;
    float z;
    float w;

    public Quaternion set (float[] mem) {
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


}
