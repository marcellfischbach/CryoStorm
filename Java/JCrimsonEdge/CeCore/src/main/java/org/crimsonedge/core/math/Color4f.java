package org.crimsonedge.core.math;

import java.util.List;

public class csColor4f {

    public float r;
    public float g;
    public float b;
    public float a;



    public static float[] toFloatArray(List<csColor4f> colors) {
        float[] result = new float[colors.size() * 4];
        int i = 0;
        for (csColor4f color : colors) {
            result[i++] = color.r;
            result[i++] = color.g;
            result[i++] = color.b;
            result[i++] = color.a;
        }
        return result;
    }
}
