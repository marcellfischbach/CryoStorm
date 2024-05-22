package org.crimsonedge.core.math;

import java.util.List;

public class Vector4f {

    public float x;

    public float y;

    public float z;

    public float w;

    public Vector4f() {
        this (0.0f, 0.0f, 0.0f, 0.0f);
    }

    public Vector4f(float x, float y, float z) {
        this (x, y, z, 1.0f);
    }


    public Vector4f(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    public static float[] toFloatArray(List<Vector4f> vectors) {
        float[] result = new float[vectors.size() * 4];
        int i = 0;
        for (Vector4f vector : vectors) {
            result[i++] = vector.x;
            result[i++] = vector.y;
            result[i++] = vector.z;
            result[i++] = vector.w;
        }
        return result;
    }
}
