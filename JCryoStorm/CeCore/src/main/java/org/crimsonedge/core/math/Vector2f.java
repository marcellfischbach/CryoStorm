package org.crimsonedge.core.math;

import java.util.List;

public class csVector2f {

    public float x;

    public float y;

    public csVector2f() {
    }

    public csVector2f(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public static float[] toFloatArray(List<csVector2f> vectors) {
        float[] result = new float[vectors.size() * 2];
        int i = 0;
        for (csVector2f vector : vectors) {
            result[i++] = vector.x;
            result[i++] = vector.y;
        }
        return result;
    }
}
