package org.crimsonedge.core.math;

import java.util.List;

public class IntArray {


    public static int[] toIntArray (List<Integer> integers) {
        int[] result = new int[integers.size()];
        int i = 0;
        for (Integer integer : integers) {
            result[i++] = integer;
        }
        return result;
    }
}
