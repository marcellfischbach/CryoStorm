package org.crimsonedge.core.math;

import java.util.List;

public class Vector2f {

    public float x;

    public float y;

    public Vector2f() {
    }

    public Vector2f(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public static float[] toFloatArray(List<Vector2f> vectors) {
        float[] result = new float[vectors.size() * 2];
        int i = 0;
        for (Vector2f vector : vectors) {
            result[i++] = vector.x;
            result[i++] = vector.y;
        }
        return result;
    }
}
