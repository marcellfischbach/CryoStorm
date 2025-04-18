package org.cryo.core.math;

public class CsQuaternion
{

    float x;
    float y;
    float z;
    float w;

    public CsQuaternion() {
        this(0.0f, 0.0f, 0.0f, 1.0f);
    }

    public CsQuaternion(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    public CsQuaternion(CsQuaternion q) {
        this.x = q.x;
        this.y = q.y;
        this.z = q.z;
        this.w = q.w;
    }

    public CsQuaternion(CsVector3f v) {
        this.x = v.x;
        this.y = v.y;
        this.z = v.z;
        this.w = 0.0f;
    }

    public CsQuaternion set(CsQuaternion q) {
        this.x = q.x;
        this.y = q.y;
        this.z = q.z;
        this.w = q.w;
        return this;
    }

    public CsQuaternion set(float[] mem) {
        this.x = mem[0];
        this.y = mem[1];
        this.z = mem[2];
        this.w = mem[3];
        return this;
    }

    public void get(float[] mem) {
        mem[0] = x;
        mem[1] = y;
        mem[2] = z;
        mem[3] = w;
    }


    public CsQuaternion invert() {
        x = -x;
        y = -y;
        z = -z;
        return this;
    }


    public void normalize() {
        float l = (float) Math.sqrt(x * x + y * y + z * z + w * w);
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    public void setAxisAngle(float axisX, float axisY, float axisZ, float angle) {
        float angle2 = angle / 2.0f;
        float c = (float) Math.cos(angle2);
        float s = (float) Math.sin(angle2);
        this.x = axisX * s;
        this.y = axisY * s;
        this.z = axisZ * s;
        this.w = c;
    }


    public void setAxisAngle(CsVector3f v, float angle) {
        setAxisAngle(v.x, v.y, v.z, angle);
    }


    public static CsQuaternion fromAxisAngle(CsVector3f v, float angle) {
        return fromAxisAngle(v.x, v.y, v.z, angle);
    }


    public static CsQuaternion fromAxisAngle(float x, float y, float z, float angle) {
        float angle2 = angle / 2.0f;
        float c = (float) Math.cos(angle2);
        float s = (float) Math.sin(angle2);
        return new CsQuaternion(x * s, y * s, z * s, c);
    }


    public static CsQuaternion fromMatrix(CsMatrix3f m) {
        float qw = (float) Math.sqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
        float qw4 = qw * 4.0f;
        return new CsQuaternion(
                (m.m21 - m.m12) / qw4,
                (m.m02 - m.m20) / qw4,
                (m.m10 - m.m01) / qw4,
                qw
        );
    }


    public static CsQuaternion fromMatrix(CsMatrix4f m) {
        float qw = (float) Math.sqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
        float qw4 = qw * 4.0f;
        return new CsQuaternion(
                (m.m21 - m.m12) / qw4,
                (m.m02 - m.m20) / qw4,
                (m.m10 - m.m01) / qw4,
                qw
        );
    }


    public CsMatrix3f toMatrix3() {
        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float sqw = w * w;
        return new CsMatrix3f(
                2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y),
                2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x),
                2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f);

    }

    public CsMatrix4f toMatrix4() {
        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float sqw = w * w;
        return new CsMatrix4f(
                2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y), 0.0f,
                2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x), 0.0f,
                2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

    }

    public CsMatrix4f toMatrix4(CsMatrix4f m) {
        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float sqw = w * w;
        m.set(
                2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y), 0.0f,
                2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x), 0.0f,
                2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
        return m;
    }


    public static CsQuaternion blend(CsQuaternion q0, CsQuaternion q1, float f) {
        float f0 = 1.0f - f;
        float f1 = f;
        return new CsQuaternion(
                q0.x * f0 + q1.x * f1,
                q0.y * f0 + q1.y * f1,
                q0.z * f0 + q1.z * f1,
                q0.w * f0 + q1.w * f1
        );
    }

    public CsQuaternion mul(float f) {
        this.x *= f;
        this.y *= f;
        this.z *= f;
        this.w *= f;
        return this;
    }

    public CsQuaternion add(CsQuaternion q) {
        this.x += q.x;
        this.y += q.y;
        this.z += q.z;
        this.w += q.w;
        return this;
    }

    public CsQuaternion sub(CsQuaternion q) {
        this.x -= q.x;
        this.y -= q.y;
        this.z -= q.z;
        this.w -= q.w;
        return this;
    }

    public CsQuaternion mul(CsQuaternion q) {

        float x = this.w * q.x + this.x * q.w + this.y * q.z - this.z * q.y;
        float y = this.w * q.y - this.x * q.z + this.y * q.w + this.z * q.x;
        float z = this.w * q.z + this.x * q.y - this.y * q.x + this.z * q.w;
        float w = this.w * q.w - this.x * q.x - this.y * q.y - this.z * q.z;
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
        return this;
    }

    public CsVector3f rotate(CsVector3f v) {
        CsQuaternion q = new CsQuaternion(this)
                .mul(new CsQuaternion(v))
                .mul(new CsQuaternion(this).invert());
        return new CsVector3f(q.x, q.y, q.z);
    }


}