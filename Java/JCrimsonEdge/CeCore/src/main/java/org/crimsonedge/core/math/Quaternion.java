package org.crimsonedge.core.math;

public final class Quaternion {

    private float x;
    private float y;
    private float z;
    private float w;

    public Quaternion() {
        x = y = z = 0.0f;
        w = 1.0f;
    }

    public void set(final Quaternion q) {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
    }

    public void set(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    public void from(Matrix3f m) {
        float qw = (float) Math.sqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
        float qw4 = qw * 4.0f;


        this.x = (m.m21 - m.m12) / qw4;
        this.y = (m.m02 - m.m20) / qw4;
        this.z = (m.m10 - m.m01) / qw4;
        this.w = qw;
    }

    public void toMatrix(Matrix3f m) {
        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float sqw = w * w;
        m.set(
                2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y),
                2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x),
                2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f);
    }

    public void toMatrix(Matrix4f m) {
        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float sqw = w * w;
        m.set(
                2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y), 0.0f,
                2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x), 0.0f,
                2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
    }
}
