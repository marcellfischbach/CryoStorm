package org.crimsonedge.core.graphics.shading;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;
import org.crimsonedge.core.math.*;

@CeClass("cryo::iShaderAttribute")
public class IShaderAttribute extends CoreObject {
    public IShaderAttribute(long ref) {
        super(ref);
    }

    private static native String nGetName(long ref);

    private static native boolean nIsValid(long ref);

    private static native void nSetArrayIndex(long ref, int index);

    private static native void nBindFloat(long ref, float value);

    private static native void nBindFloat2(long ref, float x, float y);

    private static native void nBindFloat3(long ref, float x, float y, float z);

    private static native void nBindFloat4(long ref, float x, float y, float z, float w);

    private static native void nBindInt(long ref, int value);

    private static native void nBindInt2(long ref, int x, int y);

    private static native void nBindInt3(long ref, int x, int y, int z);

    private static native void nBindInt4(long ref, int x, int y, int z, int w);

    private static native void nBindMatrix3f(long ref, float[] m);

    private static native void nBindMatrix4f(long ref, float[] m);


    public String getName() {
        return nGetName(getRef());
    }

    public boolean isValid() {
        return nIsValid(getRef());
    }

    public void setArrayIndex(int index) {
        nSetArrayIndex(getRef(), index);
    }

    public void bind(float x) {
        nBindFloat(getRef(), x);
    }

    public void bind(float x, float y) {
        nBindFloat2(getRef(), x, y);
    }

    public void bind(float x, float y, float z) {
        nBindFloat3(getRef(), x, y, z);
    }

    public void bind(float x, float y, float z, float w) {
        nBindFloat4(getRef(), x, y, z, w);
    }

    public void bind(int x) {
        nBindInt(getRef(), x);
    }

    public void bind(int x, int y) {
        nBindInt2(getRef(), x, y);
    }

    public void bind(int x, int y, int z) {
        nBindInt3(getRef(), x, y, z);
    }

    public void bind(int x, int y, int z, int w) {
        nBindInt4(getRef(), x, y, z, w);
    }


    public void bind(Vector2f v) {
        nBindFloat2(getRef(), v.x, v.y);
    }

    public void bind(Vector3f v) {
        nBindFloat3(getRef(), v.x, v.y, v.z);
    }

    public void bind(Vector4f v) {
        nBindFloat4(getRef(), v.x, v.y, v.z, v.w);
    }

    public void bind(Color4f v) {
        nBindFloat4(getRef(), v.r, v.g, v.b, v.a);
    }

    private static final float[] F9 = new float[9];

    public void bind(Matrix3f v) {
        v.toArray(F9);
        nBindMatrix3f(getRef(), F9);
    }

    private static final float[] F16 = new float[16];

    public void bind(Matrix4f v) {
        v.toArray(F16);
        nBindMatrix4f(getRef(), F16);
    }


}
