package org.crimsonedge.core.math;

import java.util.List;

public class Vector3f {

    public float x;

    public float y;

    public float z;

    public static final Vector3f ZERO = new Vector3f(0, 0, 0);
    public static final Vector3f ONE = new Vector3f(1, 1, 1);

    public static final Vector3f RIGHT = new Vector3f(1, 0, 0);
    public static final Vector3f LEFT = new Vector3f(-1, 0, 0);
    public static final Vector3f UP = new Vector3f(0, 1, 0);
    public static final Vector3f DOWN = new Vector3f(0, -1, 0);
    public static final Vector3f FORWARD = new Vector3f(0, 0, 1);
    public static final Vector3f BACKWARD = new Vector3f(0, 0, -1);

    public Vector3f() {
    }

    public Vector3f(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public static float[] toFloatArray(List<Vector3f> vectors) {
        float[] result = new float[vectors.size() * 3];
        int i = 0;
        for (Vector3f vector : vectors) {
            result[i++] = vector.x;
            result[i++] = vector.y;
            result[i++] = vector.z;
        }
        return result;
    }

    public void set(Vector3f v) {
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
