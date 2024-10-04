package org.crimsonedge.core.graphics;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.math.*;

import java.util.List;

@CeClass("cryo::iRenderMeshGenerator")
public class IRenderMeshGenerator extends CoreObject {

    public IRenderMeshGenerator(long ref) {
        super(ref);
    }

    private static native void nSetVertices2f(long ref, float[] vertices);

    private static native void nSetVertices3f(long ref, float[] vertices);

    private static native void nSetVertices4f(long ref, float[] vertices);

    private static native void nSetNormals3f(long ref, float[] normals);

    private static native void nSetColors4f(long ref, float[] colors);

    private static native void nSetTangents3f(long ref, float[] tangents);

    private static native void nSetUV02f(long ref, float[] uvs);

    private static native void nSetIndices(long ref, int[] indices);

    private static native IRenderMesh nGenerate(long ref);

    public void setVertices2f(List<csVector2f> vertices) {
        nSetVertices2f(getRef(), csVector2f.toFloatArray(vertices));
    }

    public void setVertices3f(List<csVector3f> vertices) {
        nSetVertices3f(getRef(), csVector3f.toFloatArray(vertices));
    }

    public void setVertices4f(List<csVector4f> vertices) {
        nSetVertices4f(getRef(), csVector4f.toFloatArray(vertices));
    }

    public void setNormals3f(List<csVector3f> normals) {
        nSetNormals3f(getRef(), csVector3f.toFloatArray(normals));

    }

    public void setColors4f(List<csColor4f> colors) {
        nSetColors4f(getRef(), csColor4f.toFloatArray(colors));

    }

    public void setTangents3f(List<csVector3f> tangents) {
        nSetTangents3f(getRef(), csVector3f.toFloatArray(tangents));

    }

    public void setUV02f(List<csVector2f> uvs) {
        nSetUV02f(getRef(), csVector2f.toFloatArray(uvs));

    }

    public void setIndices(List<Integer> indices) {
        nSetIndices(getRef(), IntArray.toIntArray(indices));
    }


    public IRenderMesh generate() {
        return nGenerate(getRef());
    }
}
