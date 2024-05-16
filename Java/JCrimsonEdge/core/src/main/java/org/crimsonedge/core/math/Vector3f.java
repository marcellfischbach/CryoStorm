package org.crimsonedge.core.math;

import java.util.List;

public class Vector3f {

    public float x;

    public float y;

    public float z;

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

}
