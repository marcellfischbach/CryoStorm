package org.crimsonedge.core.math;

import java.util.List;

public class csVector3f {

    public float x;

    public float y;

    public float z;

    public static final csVector3f ZERO = new csVector3f(0, 0, 0);
    public static final csVector3f ONE = new csVector3f(1, 1, 1);

    public static final csVector3f RIGHT = new csVector3f(1, 0, 0);
    public static final csVector3f LEFT = new csVector3f(-1, 0, 0);
    public static final csVector3f UP = new csVector3f(0, 1, 0);
    public static final csVector3f DOWN = new csVector3f(0, -1, 0);
    public static final csVector3f FORWARD = new csVector3f(0, 0, 1);
    public static final csVector3f BACKWARD = new csVector3f(0, 0, -1);

    public csVector3f() {
    }

    public csVector3f(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public static float[] toFloatArray(List<csVector3f> vectors) {
        float[] result = new float[vectors.size() * 3];
        int i = 0;
        for (csVector3f vector : vectors) {
            result[i++] = vector.x;
            result[i++] = vector.y;
            result[i++] = vector.z;
        }
        return result;
    }

    public void set(csVector3f v) {
        this.x = v.x;
        this.y = v.y;
        this.z = v.z;
    }


    public void set(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }


    public float length() {
        return (float) Math.sqrt(x * x + y * y + z * z);
    }
}
